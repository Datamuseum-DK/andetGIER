#!/usr/bin/env bash
set -e
sox exterminate.mp3 -c1 -r12500 exterminate.wav
python make_wav_player.py 12500 exterminate.wav exterminate.asc
python slip.py asm exterminate.asc exterminate_bin.flx
python slip.py disasm exterminate_bin.flx > exterminate_disasm.asc
echo "OK - load exterminate_bin.flx as tape in GIER emulator (https://lemo.dk/gier/), and type r< in typewriter"
