#!/usr/bin/env bash
set -e
sox -r6250 -c1 -b16 -n dither.wav synth 1 tpdfnoise bandreject 1000 700
sox impossible_mission.wav -c1 -r6250 -b16 --norm=4 impossible_mission.resamp.wav highpass 200
python make_wav_player.py 6250 impossible_mission.resamp.wav impossible_mission.asc dither.wav
python slip.py asm impossible_mission.asc impossible_mission_bin.flx
python slip.py disasm impossible_mission_bin.flx > impossible_mission_disasm.asc
echo "OK - load impossible_mission_bin.flx as tape in GIER emulator (https://lemo.dk/gier/), and type r< in typewriter"
