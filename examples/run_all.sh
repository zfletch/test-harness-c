#!/bin/bash

for file in *.c
do
	base=$(basename $file .c)
	echo $base
	./$base
	echo
done
