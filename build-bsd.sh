#!/usr/bin/env bash
cc -I/usr/local/include -O0 -g -Wall skrivemaskine.c -o skrivemaskine -L/usr/local/lib -lSDL3 -lGL -lm
