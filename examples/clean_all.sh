#!/bin/bash

for file in *.c
do
	base=$(basename $file .c)
	rm $base
done
