#!/usr/bin/env bash
set -e
cd glyphs/$1
for f in _correlate.*.png ; do
	echo $f
	gm convert $f -resize 800% _upscale-$f
done
../../04-combine $2 _upscale-*.png
mv _combine.png ../../_hi.$1.png
