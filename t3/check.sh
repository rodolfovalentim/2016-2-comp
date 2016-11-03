#!/bin/bash

DATA=/home/rodolfo/Workspace/2016-2-comp
IN1=$DATA/t2/out
IN2=$DATA/t2/my-out
OUT=$DATA/t2/"diff"

mkdir -p $OUT
rm -f $OUT/*

for infile in `ls $IN1/*.out`; do
	base=$(basename $infile)
	myresult=$IN2/$base
	outfolder=$OUT/${base/.out/.diff}
	diff $infile $myresult > $outfolder
done

for infile in `ls $OUT/*.diff`; do
	FILESIZE=$(stat -c%s "$infile")
	if [[ "$FILESIZE" == 0 ]]
	then
		echo ${infile/.diff/.cm} " -> OK"
	else
		echo ${infile/.diff/.cm} " -> Error"
	fi
done
