#!/bin/bash

DATA=/home/rodolfo/Workspace/2016-2-comp
IN=$DATA/in
OUT=$DATA/t2/my-out

EXE=./parser

rm -rf $OUT
mkdir -p $OUT

for infile in `ls $IN/*.cm`; do
    base=$(basename $infile)
    outfile=$OUT/${base/.cm/.out}
    $EXE < $infile > $outfile
done
