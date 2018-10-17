#
# Josh Hursey and Samantha Foley
# CS 441/541: Project 6
#
#####################################################################
#
# Type "make" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################
CC=gcc
CFLAGS=-Wall -g -O0
LDFLAGS=

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
all: pagefault

#
# Page Fault Simulator program
#
pagefault: pagefault.c pagefault.h
	$(CC) -o pagefault $(CFLAGS) pagefault.c

#
# Cleanup the files that we have created
#
clean:
	$(RM) pagefault
	$(RM) -rf *.dSYM

#
# Tests
#
include given-tests/Makefile
