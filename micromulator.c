#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// WorksVKM Emulator (ported from Python)

#define MA 1024

#define SM 1000

#define PS 512


int main(int argc, char **argv) {
    uint16_t M[MA]; uint16_t A = 0; uint16_t B = 0; uint16_t C = 0; uint16_t D = 0; uint16_t t = 0; uint16_t PA = MA - PS; uint16_t pc = PA; uint16_t S[SM] = {PA}; int T = 0; int SI = 0; unsigned char *P; long FL; FILE *F = fopen(argv[1], "rb");
    printf("Works Virtual Machine\n"); printf("(C) 2024-2025 DonnyWorks Software\n"); if (argc < 2) { printf("Usage: emulator [P bin]"); return -1; }
    if (F == NULL) { perror("Error opening file"); return 1; }; fseek(F, 0, SEEK_END); FL = ftell(F); rewind(F); P = (char *)malloc(FL * sizeof(char)); if (P == NULL) { perror("Error allocating M"); fclose(F); return 1; }
    fread(P, FL, 1, F); fclose(F); for (int n = 0; n < FL; n++) { M[PA + n] = P[n]; }
    while (pc < MA) {
        int op = M[pc]; int param;
        if (op == 0) {} else if (op == 0x01) { param = (M[pc + 1]*16) + M[pc + 2]; pc += 2; t += param;
        } else if (op == 0xF1) { param = (M[pc + 1]*16) + M[pc + 2]; pc += 2; t += M[param];
        } else if (op == 0x02) { param = (M[pc + 1]*16) - M[pc + 2]; pc += 2; t += param;
        } else if (op == 0xF2) { param = (M[pc + 1]*16) - M[pc + 2]; pc += 2; t += M[param];
        } else if (op == 0x03) { param = (M[pc + 1]*16) * M[pc + 2]; pc += 2; t += param;
        } else if (op == 0xF3) { param = (M[pc + 1]*16) * M[pc + 2]; pc += 2; t += M[param];
        } else if (op == 0x04) { param = (M[pc + 1]*16) / M[pc + 2]; pc += 2; t += param;
        } else if (op == 0xF4) { param = (M[pc + 1]*16) / M[pc + 2]; pc += 2; t += M[param];
        } else if (op == 0x05) { param = (M[pc + 1]*16) + M[pc + 2]; SI++; S[SI] = pc + 2; pc = PA + param - 1;
        } else if (op == 0xF5) { param = (M[pc + 1]*16) + M[pc + 2]; SI++; S[SI] = pc + 2; pc = param - 1;
        } else if (op == 0x06) { param = (M[pc + 1]*16) + M[pc + 2]; int param2 = (M[pc + 3]*16) + M[pc + 4]; if (param == t) { SI++; S[SI] = pc + 2; pc = PA - 1 + param2; } pc += 2;
        } else if (op == 0xE6) { param = M[(M[pc + 1]*16) + M[pc + 2]]; int param2 = (M[pc + 3]*16) + M[pc + 4]; if (param == t) { SI++; S[SI] = pc + 2; pc = param2 - 1; } pc += 2;
        } else if (op == 0x07) { param = (M[pc + 1]*16) + M[pc + 2]; A = param; pc += 2;
        } else if (op == 0xF7) { param = M[(M[pc + 1]*16) + M[pc + 2]]; A = param; pc += 2;
        } else if (op == 0x08) { param = (M[pc + 1]*16) + M[pc + 2]; B = param; pc += 2;
        } else if (op == 0xF8) { param = M[(M[pc + 1]*16) + M[pc + 2]]; B = param; pc += 2;
        } else if (op == 0x09) { param = (M[pc + 1]*16) + M[pc + 2]; C = param; pc += 2;
        } else if (op == 0xF9) { param = M[(M[pc + 1]*16) + M[pc + 2]]; C = param; pc += 2;
        } else if (op == 0x0A) { param = (M[pc + 1]*16) + M[pc + 2]; D = param; pc += 2;
        } else if (op == 0xFA) { param = M[(M[pc + 1]*16) + M[pc + 2]]; D = param; pc += 2;
        } else if (op == 0x0B) { M[(M[pc + 1]*16) + M[pc + 2]] = A; pc += 2;
        } else if (op == 0xFB) { M[M[(M[pc + 1]*16) + M[pc + 2]]] = A; pc += 2;
        } else if (op == 0x0C) { M[(M[pc + 1]*16) + M[pc + 2]] = B; pc += 2;
        } else if (op == 0xFC) { M[M[(M[pc + 1]*16) + M[pc + 2]]] = B; pc += 2;
        } else if (op == 0x0D) { M[(M[pc + 1]*16) + M[pc + 2]] = C; pc += 2;
        } else if (op == 0xFD) { M[M[(M[pc + 1]*16) + M[pc + 2]]] = C; 
        } else if (op == 0x0E) { M[(M[pc + 1]*16) + M[pc + 2]] = D;
        } else if (op == 0xFE) { M[M[(M[pc + 1]*16) + M[pc + 2]]] = D;
        } else if (op == 0x0F) { pc = MA;
        } else if (op == 0x10 || op == 0xFF) { if (op == 0x10) { param = (M[pc + 1]*16) + M[pc + 2]; pc += 2; }; if (op == 0xFF) { param = M[(M[pc + 1]*16) + M[pc + 2]]; pc += 2; }; if (param == 0x01) { putchar((char)A); }
        } else if (op == 0x11) { pc = S[SI]; S[SI] = 0; SI--;
        } else if (op == 0x12 || op == 0xE1) { if (op == 0x12) { param = (M[pc + 1]*16) + M[pc + 2]; pc += 2; }; if (op == 0xE1) { param = M[(M[pc + 1]*16) + M[pc + 2]]; pc += 2; }; M[param] = (t - round(t/16)*16); M[param + 1] = round(t/16); pc += 2;
        }
        pc++;
    }
    //fclose(F);
    // DONNYWORKS - Commenting that out prevents a fault but also causes a memory leak
    // Oh well
    return 0;
}
