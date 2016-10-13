#!/bin/bash

DATA=/home/rodolfo/Workspace/2016-2-comp/lab8/
IN=$DATA/in
OUT=$DATA/my-out

EXE=./parser

for infile in `ls $IN/*.tny`; do
    base=$(basename $infile)
    outfile=$OUT/${base/.tny/.dot}
    $EXE < $infile > $outfile
done
