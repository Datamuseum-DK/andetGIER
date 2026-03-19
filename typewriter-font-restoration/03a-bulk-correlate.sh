#!/usr/bin/env bash

set -e

cd $(dirname $0)

if [ -z "$1" ] ; then
	echo "usage: $0 <glyph>"
	exit 1
fi

pick="_pick_$1.png"

if [ ! -e $pick ] ; then
	echo "expected $pick to exist (make with 02-pick)"
	exit 1
fi

workdir="glyphs/$1"
mkdir -p $workdir
cp $pick "$workdir/"
cd $workdir

for f in ../../scratch/deskew-30007378-*.pbm.png ; do
#for f in ../../scratch/deskew-*.pbm.png ; do
	echo $f
	../../03-correlate $pick $f 5
done
