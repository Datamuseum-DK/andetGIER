#!/usr/bin/env bash
pkgs="sdl3 gl"
cc -DHAVE_POSIX $(pkg-config --cflags $pkgs) -O0 -g -Wall typewriter.c -o typewriter $(pkg-config --libs $pkgs) -lm
