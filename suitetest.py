#!/usr/bin/python3

import sys, os

translation = {"asm":"./works_asm.py","wvm":"./emulator.py"}

f = open("tests.txt").read().split("\n")

for i in f:
    if i != "":
        datae = i.split(" ")
        k = datae[0]
        v = datae[1]
        os.system(translation[k] + " " + v)
