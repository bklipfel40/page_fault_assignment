#!/usr/bin/perl

#
# A little script to run the project and check it against the
# expected result trace.
#

use strict;

my $binary = "./pagefault";

###########################################
# Parse Command Line Arguments
# [FRAMES] [INPUT FILE] [EXPECTED FILE]
###########################################
if( scalar(@ARGV) < 3 ) {
    print "ERROR: Not enough arguments\n";
    exit -1;
}

my $num_frames = int($ARGV[0]);
my $input_file = $ARGV[1];
my $exp_file   = $ARGV[2];
my $cmd;

if( !(-e $input_file) ) {
    print "Error: Input file does not exist ($input_file)\n";
    exit -1;
}

if( !(-e $exp_file) ) {
    print "Error: Expected file does not exist ($exp_file)\n";
    exit -2;
}


###########################################
# Diagnostics
###########################################
print "-"x70 . "\n";
print "Running the command:\n";
if( $num_frames > 0 ) {
    $cmd = "$binary -f $num_frames $input_file";
} else {
    $cmd = "$binary $input_file";
}
print "\tshell\$ $cmd\n";


###########################################
# Run the command and gather all output
###########################################
my @output;
@output = `$cmd 2>&1`;


###########################################
# Extract the values from the live run
###########################################
my @per_frame_results = ();

foreach my $line (@output) {
    chomp($line);

    if( $line =~ /# /i ) {
        if( $line =~ /\d/ ) {
            $line =~ s/#\s+//;
            push(@per_frame_results, $line);
        }
    }
}

if( scalar(@per_frame_results) == 0 ) {
    printf("Error: No per-frame results found. Make sure the final\n");
    printf("       per-frame results are prefixed with a # symbol.\n");
    display_result(1);
    #exit 1;
    exit 0;
}

if( $num_frames > 0 && scalar(@per_frame_results) != 1 ) {
    printf("Error: More than one frame results found. Make sure the final\n");
    printf("       per-frame results are prefixed with a # symbol and\n");
    printf("       only the frames requested are displayed when the -f\n");
    printf("       command line parameter is supplied\n");
    display_result(1);
    #exit 1;
    exit 0;
}

if( $num_frames <= 0 && scalar(@per_frame_results) != 7 ) {
    printf("Error: %2d frames displayed instead of exactly 7.\n", scalar(@per_frame_results));
    printf("       Make sure the final per-frame results are prefixed\n");
    printf("       with a # symbol and all frames are displayed when the -f\n");
    printf("       command line parameter is -not- supplied\n");
    display_result(1);
    #exit 1;
    exit 0;
}

#print "DEBUG: " . $num_frames . " == " . scalar(@per_frame_results) . "\n";

###########################################
# Extract the expected values from the trace
###########################################
my @exp_per_frame_results = ();

@output = `cat $exp_file`;

foreach my $line (@output) {
    chomp($line);

    if( $line =~ /# /i ) {
        if( $line =~ /\d/ ) {
            $line =~ s/#\s+//;
            push(@exp_per_frame_results, $line);
        }
    }
}

###########################################
# Check the values against expected
###########################################
my $total_errors = 0;

if( scalar(@per_frame_results) != scalar(@exp_per_frame_results) ) {
    printf("Error: %2d frames displayed instead of %2d.\n", scalar(@per_frame_results), scalar(@exp_per_frame_results));
    printf("       Make sure the final per-frame results are prefixed\n");
    printf("       with a # symbol and all necessary frames are displayed when the -f\n");
    printf("       command line parameter is -not- supplied, and only that frame\n");
    printf("       is displayed when the command line parameter is supplied.\n");
    display_result(1);
}

foreach my $pr (@per_frame_results) {
    #print "DEBUG: <".$pr.">\n";
    if( 0 != compare_per_frame_to_exp($pr) ) {
        ++$total_errors;
    }
}

foreach my $pr (@exp_per_frame_results) {
    #print "DEBUG: <".$pr.">\n";
    if( 0 != compare_per_frame_to_peer($pr) ) {
        ++$total_errors;
    }
}

###########################################
# Determine pass/fail
###########################################
display_result($total_errors);

#exit $total_errors;
exit 0;

#
# Display the result of the test
#
sub display_result() {
    my $num_errors = shift(@_);

    if( 0 == $num_errors ) {
        print "Passed!\n";
    }
    else {
        print "*"x5 . " Failed 1 test! " . "*"x5 . "\n";
    }

    return $num_errors;
}

#
# Given an 'actual' per process line, find the corresponding 'expected' line
# and compare the two results.
#
sub compare_per_frame_to_exp() {
    my $orig = shift(@_);
    my @values = split(/\s+/, $orig);
    my @exp_values = ();
    my $was_found = 0;

    #
    # Find the match for this process in the expected set
    #
    foreach my $exp_orig (@exp_per_frame_results) {
        @exp_values = split(/\s+/, $exp_orig);

        if( $exp_values[0] == $values[0] ) {
            $was_found = 1;

            #
            # Make sure all values are present
            #
            if( scalar(@exp_values) != scalar(@values) ) {
                printf("Error: Per process line does not contain exactly five numbers\n");
                printf("       Expected: %s\n", $exp_orig);
                printf("       Actual  : %s\n", $orig);
                return -99;
            }

            #
            # Check all of the values
            #
            my $total = 0;
            for(my $i = 0; $i < scalar(@exp_values); ++$i ) {
                if( $exp_values[$i] != $values[$i] ) {
                    printf("Error: Per process line at position %d is incorrect. Expected (%4d), Actual (%4d)\n",
                           ($i+1), $exp_values[$i], $values[$i]);
                    ++$total;
                }
            }

            #
            # If one or more of the values was incorrect display an error.
            #
            if( $total > 0 ) {
                printf("       Expected: %s\n", $exp_orig);
                printf("       Actual  : %s\n", $orig);
                return -1 * $total;
            }

        }
    }

    #
    # If a match was not found then issue an error
    #
    if( 0 == $was_found ) {
        printf("Error: Could not find a match for the following line in the expected output\n");
        printf("       Expected: <no match found>\n");
        printf("       Actual  : %s\n", $orig);
        printf("       Expected Output Trace File: %s\n", $exp_file);
        return -88;
    }

    return 0;
}

sub compare_per_frame_to_peer() {
    my $orig = shift(@_);
    my @values = split(/\s+/, $orig);
    my @exp_values = ();
    my $was_found = 0;

    #
    # Find the match for this process in the expected set
    #
    foreach my $exp_orig (@per_frame_results) {
        @exp_values = split(/\s+/, $exp_orig);

        if( $exp_values[0] == $values[0] ) {
            $was_found = 1;

            #
            # Make sure all values are present
            #
            if( scalar(@exp_values) != scalar(@values) ) {
                printf("Error: Per process line does not contain exactly five numbers\n");
                printf("       Expected: %s\n", $orig);
                printf("       Actual  : %s\n", $exp_orig);
                return -99;
            }

            #
            # Check all of the values
            #
            my $total = 0;
            for(my $i = 0; $i < scalar(@exp_values); ++$i ) {
                if( $exp_values[$i] != $values[$i] ) {
                    printf("Error: Per process line at position %d is incorrect. Expected (%4d), Actual (%4d)\n",
                           ($i+1), $values[$i], $exp_values[$i]);
                    ++$total;
                }
            }

            #
            # If one or more of the values was incorrect display an error.
            #
            if( $total > 0 ) {
                printf("       Expected: %s\n", $orig);
                printf("       Actual  : %s\n", $exp_orig);
                return -1 * $total;
            }

        }
    }

    #
    # If a match was not found then issue an error
    #
    if( 0 == $was_found ) {
        printf("Error: Could not find a match for the following line in the expected output\n");
        printf("       Expected: %s\n", $orig);
        printf("       Actual  : <no match found>\n");
        printf("       Expected Output Trace File: %s\n", $exp_file);
        return -88;
    }

    return 0;
}
