#!/bin/bash

rm -rf emscripten
mkdir emscripten
emcc emulator.c -o emscripten/emulator.html --preload-file sysprint.bin --pre-js emargs.js
cd emscripten
emrun emulator.html -- sysprint.bin
