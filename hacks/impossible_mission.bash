#!/usr/bin/env bash
set -e
RATE=8175
sox -R -r$RATE -c1 -b16 -n dither.wav synth 1 tpdfnoise bandreject 1200 800
sox impossible_mission.wav -c1 -r$RATE -b16 --norm=3 impossible_mission.resamp.wav highpass 200
python make_wav_player.py -o impossible_mission.asc -d dither.wav impossible_mission.resamp.wav
python slip.py asm impossible_mission.asc impossible_mission_bin.flx
python slip.py disasm impossible_mission_bin.flx > impossible_mission_disasm.asc
echo "OK - load impossible_mission_bin.flx as tape in GIER emulator (https://lemo.dk/gier/), and type r< in typewriter"
