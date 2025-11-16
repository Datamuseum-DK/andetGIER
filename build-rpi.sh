#!/usr/bin/env bash
pkgs="sdl3 gl"
cc $(pkg-config --cflags $pkgs) -O0 -g -Wall skrivemaskine.c -o skrivemaskine $(pkg-config --libs $pkgs) -lm
