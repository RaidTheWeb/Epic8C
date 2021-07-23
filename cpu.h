typedef struct CPU
{
    char D; // Standard display register
    short I; // General purpose index register
    short X;
    short Y;
    short R; // Subroutine return value register
    short Z;
    short P;
    short F;
    short E;
    short C;
    short A;
    short K; // INB return register
    short S; // Stack pointer register

    short pc; // Program Counter

    int sp; // Stack pointer

} CPU;
