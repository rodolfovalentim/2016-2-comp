#!/bin/bash

DATA=/home/rodolfo/Workspace/2016-2-comp
IN=$DATA/in
OUT=$DATA/t3/my-out

EXE=./trab3
DOT="dot -Tpdf"

#rm -rf $OUT
mkdir -p $OUT

for infile in `ls $IN/*.cm`; do
    base=$(basename $infile)
    outfile=$OUT/${base/.cm/.out}
    $EXE < $infile > $outfile
    outfile=$OUT/${base/.cm/.dot}
    $EXE < $infile > $outfile
    outfile2=$OUT/${base/.cm/.pdf}
    $DOT $outfile -o $outfile2
done
