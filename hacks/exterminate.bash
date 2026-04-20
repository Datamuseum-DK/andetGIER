#!/usr/bin/env bash
set -e
sox -r12500 -c1 -b16 -n dither.wav synth 1 tpdfnoise bandreject 1000 700
sox exterminate.mp3 -c1 -r12500 -b16 exterminate.wav
python make_wav_player.py 12500 exterminate.wav exterminate.asc dither.wav
python slip.py asm exterminate.asc exterminate_bin.flx
python slip.py disasm exterminate_bin.flx > exterminate_disasm.asc
echo "OK - load exterminate_bin.flx as tape in GIER emulator (https://lemo.dk/gier/), and type r< in typewriter"
