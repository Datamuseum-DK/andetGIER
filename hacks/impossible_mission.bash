#!/usr/bin/env bash
set -e
sox impossible_mission.wav -c1 -r6250 impossible_mission.resamp.wav
python make_wav_player.py 6250 impossible_mission.resamp.wav impossible_mission.asc
python slip.py asm impossible_mission.asc impossible_mission_bin.flx
python slip.py disasm impossible_mission_bin.flx > impossible_mission_disasm.asc
echo "OK - load impossible_mission_bin.flx as tape in GIER emulator (https://lemo.dk/gier/), and type r< in typewriter"
