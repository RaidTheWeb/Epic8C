#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>
#include <memory.h>

/**
 * @brief Initialize CPU with registers and Program Counter
 */
CPU cpu = { .D = 0x00, .I = 0x0000, .X = 0x0000, .Y = 0x0000, .R = 0x0000, .Z = 0x0000, .P = 0x0000, .F = 0x0000, .E = 0x0000, .C = 0x0000, .A = 0x0000, .K = 0x0000, .S = 0x0000, .pc = 0x000, .sp = 0 };
    
/**
 * @brief 256 entries in the stack
 */
short stack[256];

short removeTheElement(int index) {
	if (stack == NULL
		|| index < 0
		|| index >= sizeof(stack)) {
		
			return 0x00;
	}
		
	short anotherArray[sizeof(stack)];
		
	short popped = 0x00;
		
	for (int i = 0, k = 0; i < sizeof(stack); i++) {
			
		if (i == index) {
			popped = stack[i];
			continue;
		}
			
		anotherArray[k++] = stack[i];
	}
		
	memcpy(stack, &anotherArray, sizeof(stack)*sizeof(short));
	return popped;
}

int main(int argc, char **argv) {
    printf("Starting Epic8 CPU Emulator (C Port)\n");

    if(argc != 2) {
        fprintf(stderr, "usage: %s <input>\n", argv[0]);
        return 1;
    }

    unsigned char memory[1024 * 64];

    FILE *ptr;

    ptr = fopen(argv[1], "rb");

    fread(memory, 0xFFAA, 1, ptr);

    fclose(ptr);

    while(1){
        unsigned char a = memory[cpu.pc];
        unsigned char b = memory[cpu.pc + 1];
        unsigned char c = memory[cpu.pc + 2];
        unsigned char d = memory[cpu.pc + 3];

        int operation = (d | (c<<8) | (b<<16) | (a<<24));

        // TODO: Fix weird character printing
        printf("%c", '\1'); // HACK to get 0x10 to work

        switch (operation & 0xFF000000)
        {
            case 0x00000000: { // 0x0000NNNN JMP: Jump to point in memory of NNNN
                int x = operation & 0x0000FFFF;
                cpu.pc = (short)x;
                break;
            }
            
            case 0x01000000: { // 0x01FFFFFF JMP: Jump to current point in memory (effectively hanging the CPU)
                cpu.pc = (short)cpu.pc; // do nothing
                break;
            }

            case 0x02000000: { // 0x02XXXXYY MOV: Move XXXX into register YY
                int x = (operation & 0x00FFFF00) >> 8;
				int y = operation & 0x000000FF;
				
				switch(y) {
					case 0x0D: {
						cpu.S = (short) x;
						break;
					}
					
					case 0x0C: {
						cpu.K = (short) x;
						break;
					}
					
					case 0x0B: {
						cpu.A = (short) x;
						break;
					}
					
					case 0x0A: {
						cpu.C = (short) x;
						break;
					}
					
					case 0x09: {
						cpu.E = (short) x;
						break;
					}
					
					case 0x08: {
						cpu.F = (short) x;
						break;
					}
					
					case 0x07: {
						cpu.P = (short) x;
						break;
					}
					
					case 0x06: {
						cpu.Z = (short) x;
						break;
					}
					
					case 0x05: {
						cpu.R = (short) x;
						break;
					}
					
					case 0x04: {
						cpu.Y = (short) x;
						break;
					}
					
					case 0x03: {
						cpu.X = (short) x;
						break;
					}
				
					case 0x02: {
						cpu.I = (short) x;
						break;
					}
				
					case 0x01:
					default: {
						cpu.D = (char) x;
						break;
					}
				}
				cpu.pc += 4;
				break;
            }

            case 0x03000000: { // 0x030000NN CMD: Call interrupt NN
				int x = (operation & 0x000000FF);
				
				switch((char) x) {
				
					case 0x11: // clear screen and wipe buffer;
						cpu.pc += 4;
						cpu.D = 0x00;
						break;
				
					case 0x10: // push D to display buffer and write buffer as characters to display
						cpu.pc += 4;
						putchar(cpu.D);
						//cpu.D = 0x00;
						break;
						
					case 0x09: // push D to display buffer and write buffer as decimal to display
						cpu.pc += 4;
						if(cpu.D < 0x10)
							printf("%c", (char) ((char)(0x30) + cpu.D));
						else
							printf("%c", 0x30);
						cpu.D = 0x00;
						break;
						
					default:
						fprintf(stderr, "Unknown Interrupt: 0x%02x", x);
						exit(1);
						break;
				}
				break;
			}

            case 0x04000000: { // 0x04000RR PUSH: Push data onto stack
				int rr = operation & 0x000000FF;
				
				switch(rr) {
					case 0x0D: {
						stack[cpu.sp++] = cpu.S;
						break;
					}
					
					case 0x0C: {
						stack[cpu.sp++] = cpu.K;
						break;
					}
					
					case 0x0B: {
						stack[cpu.sp++] = cpu.A;
						break;
					}
				
					case 0x0A: {
						stack[cpu.sp++] = cpu.C;
						break;
					}
					
					case 0x09: {
						stack[cpu.sp++] = cpu.E;
						break;
					}
					
					case 0x08: {
						stack[cpu.sp++] = cpu.F;
						break;
					}
					
					case 0x07: {
						stack[cpu.sp++] = cpu.P;
						break;
					}
					
					case 0x06: {
						stack[cpu.sp++] = cpu.Z;
						break;
					}
					
					case 0x05: {
						stack[cpu.sp++] = cpu.R;
						break;
					}
					
					case 0x04: {
						stack[cpu.sp++] = cpu.Y;
						break;
					}
					
					case 0x03: {
						stack[cpu.sp++] = cpu.X;
						break;
					}
				
					case 0x02: {
						stack[cpu.sp++] = cpu.I;
						break;
					}
				
					case 0x01:
					default: {
						stack[cpu.sp++] = (short) cpu.D;
						break;
					}
				}
				cpu.pc += 4;
				break;
			}

            case 0x05000000: { // 0x050000RR POP: Pop data from stack into register
				int rr = operation & 0x000000FF;
				
				switch(rr) {
					
					case 0x0D: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.S = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x0C: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.K = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x0B: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.A = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x0A: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.C = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x09: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.E = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x08: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.F = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x07: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.P = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x06: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.Z = removeTheElement(cpu.sp);
						}
						break;
					}
				
					case 0x05: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.R = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x04: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.Y = removeTheElement(cpu.sp);
						}
						break;
					}
					
					case 0x03: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.X = removeTheElement(cpu.sp);
						}
						break;
					}
				
					case 0x02: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.I = removeTheElement(cpu.sp);
						}
						break;
					}
				
					case 0x01:
					default: {
						if(cpu.sp > 0) {
							cpu.sp -= 1;
							cpu.D = (char)removeTheElement(cpu.sp);
						}
						break;
					}
				}
				cpu.pc += 4;
				break;
			}

            case 0x06000000: { // 0x0600NNNN PUSH: Push hex data to stack
				short nnnn = (short)(operation & 0x0000FFFF);
				
				stack[cpu.sp] = nnnn;
				cpu.sp++;
				
				cpu.pc += 4;
				break;
			}

            case 0x07000000: { // 0x070000RR JNULL: Skip next instruction if RR equals 0x00
				int rr = operation & 0x000000FF;
				switch(rr) {
					case 0x0D: {
						if(cpu.S == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x0C: {
						if(cpu.K == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x0B: {
						if(cpu.A == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x0A: {
						if(cpu.C == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x09: {
						if(cpu.E == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x08: {
						if(cpu.F == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x07: {
						if(cpu.P == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x06: {
						if(cpu.Z == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
				
					case 0x05: {
						if(cpu.R == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x04: {
						if(cpu.Y == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
					
					case 0x03: {
						if(cpu.X == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
				
					case 0x02: {
						if(cpu.I == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
				
					case 0x01: {
						if(cpu.D == 0x00) {
							cpu.pc += 8;
						} else {
							cpu.pc += 4;
						}
						break;
					}
				}
				break;
			}

            case 0x08000000: { // 0x08000000 PEEK: Get data from stack into register without changing stack pointer
				int rr = operation & 0x000000FF;
				
				switch(rr) {
					case 0x0D: {
						if(cpu.sp > 0) {
							cpu.S = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x0C: {
						if(cpu.sp > 0) {
							cpu.K = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x0B: {
						if(cpu.sp > 0) {
							cpu.A = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x0A: {
						if(cpu.sp > 0) {
							cpu.C = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x09: {
						if(cpu.sp > 0) {
							cpu.E = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x08: {
						if(cpu.sp > 0) {
							cpu.F = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x07: {
						if(cpu.sp > 0) {
							cpu.P = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x06: {
						if(cpu.sp > 0) {
							cpu.Z = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x05: {
						if(cpu.sp > 0) {
							cpu.R = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x04: {
						if(cpu.sp > 0) {
							cpu.Y = stack[cpu.sp - 1];
						}
						break;
					}
					
					case 0x03: {
						if(cpu.sp > 0) {
							cpu.X = stack[cpu.sp - 1];
						}
						break;
					}
				
					case 0x02: {
						if(cpu.sp > 0) {
							cpu.I = stack[cpu.sp - 1];
						}
						break;
					}
				
					case 0x01:
					default: {
						if(cpu.sp > 0) {
							cpu.D = (char)stack[cpu.sp - 1];
						}
						break;
					}
				}
				cpu.pc += 4;
				break;
			}

            case 0x09000000: { // 0x090000RR POPL: FIFO stack pop into register
				int rr = operation & 0x000000FF;
				
				switch(rr) {
					case 0x0D: {
						cpu.S = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x0C: {
						cpu.K = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x0B: {
						cpu.A = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x0A: {
						cpu.C = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x09: {
						cpu.E = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x08: {
						cpu.F = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x07: {
						cpu.P = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x06: {
						cpu.Z = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x05: {
						cpu.R = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x04: {
						cpu.Y = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
					
					case 0x03: {
						cpu.X = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
				
					case 0x02: {
						cpu.I = removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
				
					case 0x01:
					default: {
						cpu.D = (char) removeTheElement(0);
						if(cpu.sp > 0) cpu.sp--;
						break;
					}
				}
				cpu.pc += 4;
				break;
			}



            case 0xFF000000: {
				switch(operation & 0x0000000F) {
					case 0x0000000F: { // FF00000F HALT: Stop CPU
						printf("Recieved Halt Operation 0xFF00000F\n");
						exit(0);
						break;
					}
					default:
						break;
				}
				break;
			}

            default:
                fprintf(stderr, "Illegal operation 0x%08x exiting with status '1'\n", operation);
                exit(1);
                break;
        }
    }
    return 0;
}