# CS441/541 Project 6

## Author(s):

Kala Arentz, Ben Klipfel


## Date:

April 12, 2018


## Description:

This program is a a memory management simulator for page replacement. This program will be reading from a file the following information: number of page references, page being referenced, and if that page was written to or read from. A command line parameter will pick the file name, and set the number of frames.  

The given command line arguments can be given in any order:  
  1. Optional: -f # : The number of frames to use in the algorithm analysis. If this parameter is not provided then your program will run for the full range of frames. The number of frames can range from 1 to 7, inclusive.  
  2. All other command line options can be considered file inputs  

You will need to implement the following page replacement algorithms (in this order).
  1. Optimal (OPT)  
  2. First-In, First Out (FIFO)  
  3. Least Recently Used (LRU)  
  4. LRU Second Chance (LRU SC)  
  5. LRU Enhanced Second Chance (LRU ESC)  


The file format provides all of the information for the pages that you will need to run these algorithms - the file describes the reference string. The first line of the file contains a single number identifying the number of page references that are contained in the file. The number of page references will range from 1 to an undefined upper bound (your program will need to handle a large number of references). You may assume that the file is formatted correctly.

The subsequent lines in the file each describe one page reference page number (integers ranging from 0 to 9, inclusive), if the page is written to or read from (indicated by the character �W� or �R� - may be in upper or lower case). Below is an example of the file given:  
```
5
4R
1w
3r
2W
4r
```
The information that will be printed out to the user will follow this format:  
```
Num. Frames : All
Ref. File   : given-tests/level1.txt
-------------------------------
Reference String:
4:R, 1:W, 3:R, 2:W, 4:R
-------------------------------
####################################################
#       Frames    Opt.    FIFO    LRU     SC     ESC
#            1       5       5      5      5       5
#            2       4       5      5      5       5
#            3       4       5      5      5       5
#            4       4       4      4      4       4
#            5       4       4      4      4       4
#            6       4       4      4      4       4
#            7       4       4      4      4       4
####################################################
```

## How to build the software

To build the software, you will run the command  
```
make
```  
If no errors are created, the message will print  
```
gcc -o pagefault -Wall -g -O0 pagefault.c
```  

Before updating the Bitbucket repository, run the command  
```
make clean
```  
If no errors are created, the message will print in the console  
```
rm -f pagefault
rm -f -rf *.dSYM
```


## How to use the software

To use the software you will run the command
```
./pagefault given-test/level1.txt
```  
This is the most general command given. A file must be given to fun. The output for this given command will be:  
```
Num. Frames : All
Ref. File   : given-tests/level1.txt
-------------------------------
Reference String:
4:R, 1:W, 3:R, 2:W, 4:R
-------------------------------
####################################################
#       Frames    Opt.    FIFO    LRU     SC     ESC
#            1       5       5      5      5       5
#            2       4       5      5      5       5
#            3       4       5      5      5       5
#            4       4       4      4      4       4
#            5       4       4      4      4       4
#            6       4       4      4      4       4
#            7       4       4      4      4       4
####################################################
```

Another command to be given is the OPTIONAL -f argument. This is the number of frames to use in the algorithm analysis. For example, to run this command:  
```
./pagefault -f 2 given-test/level1.txt
```  
The -f command may be given in any order.


## How the software was tested

#### Command Line Testing
These are the commands that are run to test the command line arguments.

If you run this command:  
```
./pagefault given-test/level1.txt -f -23
```  
The following error will be produced:  
```
Error: Must supply an integer argument between 1 and 7 for the -f option
```  

In addition, it does not matter which order you pass the -f option.

```
./pagefault given-test/level1.txt -f 2
```  
or  
```
./pagefault -f 4 given-test/level1.txt
```

Another test run is that you have given a file that can be opened. If you run the command:  
```
./pagefault given-test/level1.txt
```
The error will be provided, and will look like:
```
Error: Cannot open the file: given-test/level1.txt
```

Another test run is making sure the user gave a file to read, because if no file is given, we cannot do the algorithms.  
```
./pagefault
```  
The error that will be displayed will be:  
```
Error: You must supply a file to read!
```  

## Test Suite

This will give the basic information about the test created for this project. To find more in-depth information, please look at the tests/README.md file.

#### Test 1: large_amount_pages.txt
This will test just when you have a large amount of pages. It handles many different pages.

#### Test 2: small_amount_pages.txt
We decided that we should make a test with just a couple pages to make sure that our program was replacing spots correctly.

#### Test 3: all_reads.txt
This test will test that our program can handle all reads. This was an important test for the enchanced second chance because we were having some issues with the read and writes.

#### Test 4: all_writes.txt
This test should handle only writes.

#### Test 5: standard_test.txt
This is just a standard test of both writes and reads, helps with formatting as well.

## Known bugs and problem areas

None
