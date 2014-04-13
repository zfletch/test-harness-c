#!/bin/bash

CC=gcc

for file in *.c
do
	base=$(basename $file .c)
	$CC -Wall ../src/harness.c $file -o $base
done
