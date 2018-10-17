# CS441/541 Project 4 Test Suite

This holds all the additional information for the tests created.

#### Test 1: large_amount_pages.txt
This will test just when you have a large amount of pages. It handles many different pages.

Call command:  
```
./pagefault tests/large_amount_pages.txt
```

This is the corresponding frames section that should be printed on the console.  
```
####################################################
#     Frames    Opt.    FIFO      LRU      SC    ESC
#          1    2679    2679     2679    2679    2679
#          2    1879    2367     2352    2367    2372
#          3    1382    2034     2061    2052    2046
#          4    1017    1698     1698    1695    1743
#          5     715    1339     1375    1325    1325
#          6     473    1032     1048    1030    1062
#          7     291     705     722     691     703
####################################################
```

#### Test 2: small_amount_pages.txt
We decided that we should make a test with just a couple pages to make sure that our program was replacing spots correctly.

Call command:  
```
./pagefault tests/small_amount_pages.txt
```

This is the corresponding frames section that should be printed on the console.  
```
####################################################
#     Frames    Opt.    FIFO    LRU    SC    ESC
#          1       2       2       2       2       2
#          2       2       2       2       2       2
#          3       2       2       2       2       2
#          4       2       2       2       2       2
#          5       2       2       2       2       2
#          6       2       2       2       2       2
#          7       2       2       2       2       2
####################################################
```

#### Test 3: all_reads.txt
This test will test that our program can handle all reads. This was an important test for the enchanced second chance because we were having some issues with the read and writes.

Call command:  
```
./pagefault tests/all_reads.txt
```

This is the corresponding frames section that should be printed on the console.  
```
####################################################
#     Frames    Opt.    FIFO    LRU    SC    ESC
#          1      40      40      40      40      40
#          2      36      40      40      40      40
#          3      32      40      40      40      40
#          4      28      40      40      40      40
#          5      25      40      40      40      40
#          6      22      40      40      40      40
#          7      19      40      40      40      40
####################################################
```

#### Test 4: all_writes.txt
This test should handle only writes.

Call command:  
```
./pagefault tests/all_writes.txt
```

This is the corresponding frames section that should be printed on the console.  
```
####################################################
#     Frames    Opt.    FIFO    LRU    SC    ESC
#          1      40      40      40      40      40
#          2      36      40      40      40      40
#          3      32      40      40      40      40
#          4      28      40      40      40      40
#          5      25      40      40      40      40
#          6      22      40      40      40      40
#          7      19      40      40      40      40
####################################################
```

#### Test 5: standard_test.txt
This is just a standard test of both writes and reads, helps with formatting as well.

Call command:  
```
./pagefault tests/standard_test.txt
```

This is the corresponding frames section that should be printed on the console.  
```
####################################################
#     Frames    Opt.    FIFO    LRU    SC    ESC
#          1      29      29      29      29      29
#          2      20      29      29      29      29
#          3      12      29      29      29      20
#          4       4       4       4       4       4
#          5       4       4       4       4       4
#          6       4       4       4       4       4
#          7       4       4       4       4       4
####################################################
```
