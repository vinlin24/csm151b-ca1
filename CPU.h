#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class Instruction
{
public:
    Instruction();
    Instruction(std::bitset<32> fetch);
    /**
     * The underlying 32-bit bit pattern representing the instruction.
     */
    std::bitset<32> instruction;
};

class CPU
{
private:
    /**
     * Data memory byte addressable in Little Endian fashion.
     */
    int dataMemory[4096];
    /**
     * Program counter.
     */
    unsigned long PC;

public:
    CPU();
    /**
     * Read the current value of the program counter (PC).
     */
    unsigned long readPC();
    /**
     * Fetch the current 32-bit instruction from the instruction memory.
     */
    std::bitset<32> fetch(std::bitset<8> *instructionMemory);
    /**
     * Decodes the given instruction and returns whether the CPU is still
     * active i.e. `false` if the program should terminate.
     */
    bool decode(Instruction *instruction);
};
