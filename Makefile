################################################################################
#              Makefile for the CS 3410 Cache Blocking Project
################################################################################
#
# USAGE:  make <task>  [BLOCKFLAG=<block size>] [SHORTC]
#
#  BLOCK sets the size of value of the BLOCKSZ flag for the *.c files.  No effort is made 
#   to validate the value passed,  (should be a positive integer).
#  SC controls the value of SHORT_CIRCUIT.  Useful only in testing.
#  OPT_LEVEL sets a value for the -O flag.No effort is made to check this as a valid option.
#
BLOCK ?= 1
OPT_LEVEL ?= 3
SC ?= 0

RV = docker run -i -t --rm -v `pwd`:/root ghcr.io/sampsyo/cs3410-infra

CC = gcc
CFLAGS = -std=c17 -D_GNU_SOURCE -DBLOCKSZ=$(BLOCK) -DSHORT_CIRCUIT=$(SC) -Wall -Wextra -Wpedantic -Wshadow -g3 -O$(OPT_LEVEL)
LFLAGS = -lm


.PHONY: all clean run

all: clean matmult transpose
test: test_matmult test_transpose
matmult: helpers.o tasks.o
	$(CC) $(CFLAGS) -o $@ $@.c $^ $(LFLAGS)

transpose: helpers.o tasks.o
	$(CC) $(CFLAGS) -o $@ $@.c $^ $(LFLAGS)

test_matmult: helpers.o tasks.o
	$(CC) $(CFLAGS) -o $@ $@.c $^ $(LFLAGS)

test_transpose: helpers.o tasks.o
	$(CC) $(CFLAGS)  -o $@ $@.c $^ $(LFLAGS)

# Wildcard rule that allows for the compilation of a *.c file to a *.o file
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Removes any executables and compiled object files
clean:
	rm -f matmult transpose test_matmult test_transpose *.o
