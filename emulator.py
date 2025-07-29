#!/usr/bin/python3
print("Works Virtual Machine")
print("(C) 2024-2025 DonnyWorks Software")
import sys

memory = []
malloc_max = 1024 # 1k memory

b0 = 0 # A
b1 = 0 # B
b2 = 0 # C
b3 = 0 # D
t = 0 # TEMP
stack_max = 1000
for i in range(0,malloc_max):
    memory.append(0)
program_alloc = malloc_max - 512 # 512b of allocated memory for program.
pc = program_alloc
program = open(sys.argv[1],"rb").read() # TODO: Add case handler for no params
stack = [program_alloc]
running = True
for n in range(0,len(program)):
    memory[program_alloc + n] = program[n]
try:
    while pc < len(memory) and running:
        if len(stack) > stack_max:
            print("STACK OVERFLOW")
            raise Exception("stack overflow")
        if b0 > 65535:
            print("Emulator warning: over bounds!")
            b0 = 0
        if b1 > 65535:
            print("Emulator warning: over bounds!")
            b1 = 0
        if b2 > 65535:
            print("Emulator warning: over bounds!")
            b2 = 0
        if b3 > 65535:
            print("Emulator warning: over bounds!")
            b3 = 0
        if t > 65535:
            print("Emulator warning: over bounds!")
            t = 0
        op = memory[pc]
        # add program env here
        if op == 0x01:
            param = (memory[pc + 1]*16) + memory[pc + 2]
            pc += 2
            t += param
        elif op == 0x02:
            param = (memory[pc + 1]*16) - memory[pc + 2]
            pc += 2
            t += param
        elif op == 0x03:
            param = (memory[pc + 1]*16) * memory[pc + 2]
            pc += 2
            t += param
        elif op == 0x04:
            param = (memory[pc + 1]*16) / memory[pc + 2]
            pc += 2
            t += param
        elif op == 0xF2:
            param = (memory[pc + 1]*16) - memory[pc + 2]
            pc += 2
            t += memory[param]
        elif op == 0xF3:
            param = (memory[pc + 1]*16) * memory[pc + 2]
            pc += 2
            t += memory[param]
        elif op == 0xF4:
            param = (memory[pc + 1]*16) / memory[pc + 2]
            pc += 2
            t += memory[param]
        elif op == 0x00:
            pass
        elif op == 0xF1:
            param = (memory[pc + 1]*16) + memory[pc + 2]
            t += memory[param]
            pc += 2
        elif op == 0x05:
            #print("Jump (Local Space)")
            param = (memory[pc + 1]*16) + memory[pc + 2]
            stack.append(pc + 2)
            pc = program_alloc - 1 + param
        elif op == 0xF5:
            #print("Jump (Memory Space)")
            param = (memory[pc + 1]*16) + memory[pc + 2]
            stack.append(pc + 2)
            pc = param - 1
        elif op == 0x10 or op == 0xFF:
            if op == 0x10:
                param = (memory[pc + 1]*16) + memory[pc + 2]
                pc += 2
            if op == 0xFF:
                param = memory[(memory[pc + 1]*16) + memory[pc + 2]]
                pc += 2
            if param == 0x01:
                print(chr(b0),end='')
        elif op == 0x12 or op == 0xE1:
            if op == 0x12:
                param = (memory[pc + 1]*16) + memory[pc + 2]
            else:
                param = memory[(memory[pc + 1]*16) + memory[pc + 2]]
            memory[param] = (t - round(t/16)*16)
            memory[param + 1] = round(t/16)
            pc += 2
        elif op == 0x07: #"sta":[b"\x07",b"\xF7"]
            b0 = (memory[pc + 1]*16) + memory[pc + 2]
            pc += 2
        elif op == 0xF7:
            b0 = memory[(memory[pc + 1]*16) + memory[pc + 2]]
            pc += 2
        elif op == 0x0F:
            print("System HALT")
            running = False
        elif op == 0x11:
            #print("Return from Subroutine")
            pc = stack[len(stack) - 1]
            stack.pop()
        else:
            raise Exception("We're all doomed!")
        pc += 1
except Exception as e:
    print("WVM crashed! A [non]detailed stack trace is available. (Top = earliest, bottom = latest)")
    if str(e) == "We're all doomed!":
        print("Reason of error: Invalid operation, go fuck yourself. (" + str(hex(op)) + ")")
    try:
        symbols = open(sys.argv[1].replace(".bin",".dbg")).read().split("\n")
    except:
        print("Failed to load debug symbols.")
        symbols = []
    for i in stack:
        toprint = str(i-program_alloc)
        for sym in symbols:
            if sym != "":
                if len(sym.split("=")) > 1:
                    d = sym.split("=")
                    if int(d[1]) == i-program_alloc or int(d[1]) == i-program_alloc+1:
                        toprint = d[0]
        print(toprint)
    print("Buffers:")
    print(b0, b1, b2, b3, t)
    a = open("allocated.bin","wb")
    data = b""
    ipos = 0
    for i in memory:
        try:
            data += i.to_bytes(1,"big")
        except:
            print("Failed to print byte at", ipos)
            data += b"\xFF"
            print("Byte:", i)
        ipos += 1
    a.write(data)
    a.close()
    print("Memory has been dumped to allocated.bin")
