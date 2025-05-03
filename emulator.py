#!/usr/bin/python3

import sys

memory = []
malloc_max = 1024 # 1k memory

b0 = 0 # A
b1 = 0 # B
b2 = 0 # C
b3 = 0 # D

for i in range(0,malloc_max):
    memory.append(0)
pc = 0
program_alloc = malloc_max - 512 # 512b of allocated memory for program.
program = open(sys.argv[1],"rb").read() # TODO: Add case handler for no params
stack = [program_alloc]
for n in range(0,len(program)):
    memory[program_alloc + n] = program[n]
try:
    while pc < len(memory):
        op = memory[pc]
        # add program env here
        pc += 1
except:
    print("WVM crashed! A [non]detailed stack trace is available.")
    for i in stack:
        print(i)
    print("Buffers:")
    print(b0, b1, b2, b3)
    a = open("allocated.bin","wb")
    data = b""
    for i in memory:
        data += i.to_bytes(1,"big")
    a.write(data)
    a.close(0)
    print("Memory has been dumped to allocated.bin")
