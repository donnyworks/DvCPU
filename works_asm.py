#!/usr/bin/python3

import sys

indata = open(sys.argv[1]).read().split("\n")
outdata = open(sys.argv[1].replace(".wka",".bin"),"wb")
out = open(sys.argv[1].replace(".wka",".dbg"),"w")
data = b""
headers = {}
cmpos = 0
data_dbg = ""
operations = {"add":[b"\x01",b"\xF1"],"sub":[b"\x02",b"\xF2"],"mul":[b"\x03",b"\xF3"],"div":[b"\x04",b"\xF4"],"jmp":[b"\x05",b"\xF5"],"sta":[b"\x07",b"\xF7"],"stb":[b"\x08",b"\xF8"],"stc":[b"\x09",b"\xF9"],"std":[b"\x0A",b"\xFA"],"lda":[b"\x0B",b"\xFB"],"ldb":[b"\x0C",b"\xFC"],"ldc":[b"\x0D",b"\xFD"],"ldd":[b"\x0E",b"\xFE"],"scl":[b"\x10",b"\xFF"],"jie":[b"\x06",b"\xE6"],"wrt":[b"\x11",b"\xE1"]}
for n in indata:
    x = n.split(" ")
    i = x[0]
    i = i.replace("	","")
    if i.startswith("."):
        print("Header hit " + i)
        headers[i.replace(".","").replace(":","")] = cmpos - 1
    if i.lower() == "jie":
        cmpos += 4
        #print(i.lower())
    if i.lower() == "db":
        outfda = n.split(' ',1)[1]
        fsd = outfda.replace('"','')
        cmpos += len(fsd) + 1
    if i.lower() == "add" or i.lower() == "sub" or i.lower() == "mul" or i.lower() == "div" or i.lower() == "jmp" or i.lower() == "sta" or i.lower() == "stb" or i.lower() == "stc" or i.lower() == "std" or i.lower() == "lda" or i.lower() == "ldb" or i.lower() == "ldc" or i.lower() == "ldd" or i.lower() == "scl":
        cmpos += 2
    cmpos += 1
cmpos = 0
for n in indata:
    x = n.split(" ")
    i = x[0]
    i = i.replace("	","")
    if i.startswith("."):
        print("Header hit " + i + " at " + str(cmpos))
        headers[i.replace(".","").replace(":","")] = cmpos - 1
        data_dbg += i.replace(":","") + "=" + str(cmpos) + "\n"
    if i.lower() == "nul" or i.lower() == "nop":
        data += b"\x00"
    if i.lower() == "rts":
        data += b"\x11"
    if i.lower() == "jie":
        if x[1].startswith("#"):
            data += operations[i.lower()][0]
        if x[1].startswith("$"):
            data += operations[i.lower()][1]
        data += int(x[1].replace("$","").replace("#",""),base=16).to_bytes(2,"big")
        data += int(x[2].replace("$","").replace("#",""),base=16).to_bytes(2,"big")
        cmpos += 4
        #print(i.lower())
    if i.lower() == "db":
        outfda = n.split(' ',1)[1]
        fsd = outfda.replace('"','')
        data += fsd.encode()
        data += b"\x00"
        cmpos += len(fsd) + 1
    if i.lower() == "hlt":
        data += b"\x0F"
    if i.lower() == "add" or i.lower() == "sub" or i.lower() == "mul" or i.lower() == "div" or i.lower() == "jmp" or i.lower() == "sta" or i.lower() == "stb" or i.lower() == "stc" or i.lower() == "std" or i.lower() == "lda" or i.lower() == "ldb" or i.lower() == "ldc" or i.lower() == "ldd" or i.lower() == "scl" or i.lower() == "wrt":
        if x[1].startswith("#"):
            data += operations[i.lower()][0]
        if x[1].startswith("$"):
            data += operations[i.lower()][1]
        if x[1].startswith("."):
            x[1] = str(headers[x[1].replace('.',"")])
            if i.lower() == "jmp": data += operations[i.lower()][0]
            if i.lower() != "jmp": data += operations[i.lower()][1]
            data += int(x[1]).to_bytes(2,"big")
            #print("Ref header at " + x[1])
        else:
            data += int(x[1].replace("$","").replace("#",""),base=16).to_bytes(2,"big")
        #print("Add two from " + i)
        cmpos += 2
    #print(cmpos)
    cmpos += 1
out.write(data_dbg)
out.close()
outdata.write(data)
outdata.close()
