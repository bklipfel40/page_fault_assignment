import os, sys
import subprocess

# globals

tests = []

#DEBUG = True
DEBUG = False

def generate_test_list(fname):
    #tests = []
    try:
        testinfo = open(fname, "r").readlines()
        for line in testinfo:
            if not line[0] == "#":
                vals = line.split(",")
                if len(vals) == 4:
                    tests.append({"executable" : vals[0],
                                  "input"      : vals[1],
                                  "output"     : vals[2],
                                  "points"     : int(vals[3]),
                                  "score"      : 0})
                else:
                    print "Abort. Problem parsing <" + line + "> in " + fname
        if DEBUG:
            print tests
    except:
        print "Abort. Problem reading test info from " + fname
            


def custom_tester(output, reference, points):
    line_no = 1
    for line_out, line_ref in zip(output, reference):
        lo_chunks = line_out.split()
        lr_chunks = line_ref.split()
        # check if lengths are the same
        len_diff = len(lo_chunks) - len(lr_chunks)
        if len_diff < 0:
            print  "Not enough values on line " + str(line_no)
            print  "<" + line_out
            print  "-----"
            print  ">" + line_ref
            points = 0
        elif len_diff > 0:
            print  "Too many values on line " + str(line_no)
            print  "<" + line_out
            print  "-----"
            print  ">" + line_ref
            points = 0
        # check values in each position
        pos = 1
        for o_chunk, l_chunk in zip(lo_chunks, lr_chunks):
            if not (o_chunk == l_chunk):
                print  "Value " + o_chunk + " incorrect in position " + str(pos) + ".  Expected " + l_chunk
                print  "<" + line_out
                print  "-----"
                print  ">" + line_ref
                points = 0
            pos = pos + 1
        
    return points



def run_test():
    score = 0
    out_of = 0

    # run test cases
    test_num = 0
    for v in tests:
        test_num = test_num + 1
        # setup for run
        out_of = out_of + v["points"]
        print "Running test " + str(test_num) + ": " + v["executable"]
        # run the code with no input
        if v["input"]:
            run_code = v["executable"] + " < " + v["input"] + " > test_out"
        else:
            run_code = v["executable"] + " > test_out"
        try:
            subprocess.call(run_code, shell=True)
            test_lines = open("test_out").readlines()
            correct_lines = open(v["output"]).readlines()
        
            v["score"] = v["points"]
        
            len_diff = len(test_lines) - len(correct_lines)
            if len_diff > 0:
                print "Test file had too much data."
                for line_out in test_lines:
                    print  "<" + line_out
                print  "-----"
                for line_ref in correct_lines:
                    print  ">" + line_ref
                v["score"] = 0
            
            elif len_diff < 0:
                print "Test file had too little data."
                for line_out in test_lines:
                    print  "<" + line_out
                print  "-----"
                for line_ref in correct_lines:
                    print  ">" + line_ref
                v["score"] = 0
            else:  # len_diff == 0
                v["score"] = custom_tester(test_lines, correct_lines, v["points"])
        except:
            print "Unexpected error"
            v["score"] = 0

        score = score + v["score"]


    print "Autograde score: " + str(score) + " / " + str(out_of)


if __name__ == "__main__":
    # create list of test cases from file given on commandline
    if len(sys.argv) == 2:
        generate_test_list(sys.argv[1])
        # run the tests
        run_test()
    else:
        print "Abort. No test list provided."
