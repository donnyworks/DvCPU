#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// WorksVKM Emulator (ported from Python)

#define malloc_max 1024

#define stack_max 1000

#define program_size 512


int main(int argc, char **argv) {
    printf("Works Virtual Machine\n");
    printf("(C) 2024-2025 DonnyWorks Software\n");
    uint16_t memory[malloc_max];
    uint16_t b0 = 0;
    uint16_t b1 = 0;
    uint16_t b2 = 0;
    uint16_t b3 = 0;
    uint16_t t = 0;
    uint16_t program_alloc = malloc_max - program_size;
    uint16_t pc = program_alloc;
    uint16_t stack[stack_max] = {program_alloc};
    int count = 0;
    int stackIndex = 0;
    unsigned char *program;
    if (argc < 2) {
        printf("Usage: emulator [program bin]");
        return -1;
    }
    long file_size;
    //printf(argv[1]);
    FILE *fptr = fopen(argv[1], "rb");
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Determine the file size
    fseek(fptr, 0, SEEK_END);
    file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    if (fptr == NULL) {
        perror("Failed to open");
        return -1;
    }
    program = (unsigned char *)malloc(file_size);
    if (program == NULL) {
        perror("Memory allocation failed");
        //fclose(fptr);
        return -1;
    }
    fclose(fptr);
    size_t bytes_read = fread(program, 1, file_size, fptr);
    if (bytes_read != file_size) {
        fprintf(stderr, "Error reading file: read %zu bytes, expected %ld\n", bytes_read, file_size);
        free(program);
        //fclose(fptr);
        return -1;
    }
    for (int i = 0; i < (file_size < 10 ? file_size : 10); i++) {
        printf("%d ", program[i]);
    }
    for (int n = 0; n < file_size; n++) {
        printf("hi");
        memory[program_alloc + n] = program[n];
    }
    while (pc < malloc_max) {
        int op = memory[pc];
        int param;
        //printf("PC: %i\n", pc);
        //printf("OP: %i\n", op);
        //todo: emulator bullshit
        //printf("todo: emulator\n");
        if (op == 0) {
        } else if (op == 0x01) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF1) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x02) {
            param = (memory[pc + 1]*16) - memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF2) {
            param = (memory[pc + 1]*16) - memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x03) {
            param = (memory[pc + 1]*16) * memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF3) {
            param = (memory[pc + 1]*16) * memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x04) {
            param = (memory[pc + 1]*16) / memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF4) {
            param = (memory[pc + 1]*16) / memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x05) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            stackIndex++;
            stack[stackIndex] = pc + 2;
            pc = program_alloc - 1 + param;
        } else if (op == 0xF5) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            stackIndex++;
            stack[stackIndex] = pc + 2;
            pc = param - 1;
        } else if (op == 0x0F) {
            pc = malloc_max;
        } else if (op == 0x10 || op == 0xFF) {
            if (op == 0x10) {
                param = (memory[pc + 1]*16) + memory[pc + 2];
                pc += 2;
            }
            if (op == 0xFF) {
                param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
                pc += 2;
            }
            if (param == 0x01) {
                printf((char)param);
            }
        } else if (op == 0x11) {
            pc = stack[stackIndex];
            stack[stackIndex] = 0;
            stackIndex--;
        }
        pc++;
    }
    fclose(fptr);
    return 0;
}
