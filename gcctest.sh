#!/bin/bash

gcc emulator.c -fsanitize=address -o emulator
./emulator ./sysprint.bin
