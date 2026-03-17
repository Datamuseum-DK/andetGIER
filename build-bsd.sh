#!/usr/bin/env bash
cc -I/usr/local/include -O0 -g -Wall typewriter.c -o typewriter -L/usr/local/lib -lSDL3 -lGL -lm -pthread
