#! /usr/bin/env bash

mkdir -p bin/

gcc src/main.c -o bin/main -lSDL2_image $(pkg-config sdl2 --cflags) $(pkg-config sdl2 --libs)
