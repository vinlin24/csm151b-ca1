#include <bitset>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "ALU.h"
#include "Controller.h"
#include "ImmGen.h"
#include "RegFile.h"

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
    /**
     * Controller unit responsible for decoding the opcode part of the
     * instruction and setting the control signals depending on the type of
     * instruction.
     */
    Controller controller;
    /**
     * Register file unit responsible for keeping track of the values currently
     * stored in the registers.
     */
    RegFile regfile;
    /**
     * Immediate generator unit responsible for extracting and sign extending
     * immediates encoded in the instruction.
     */
    ImmGen immgen;
    /**
     * Unit responsible for resolving the ALUOp control signal and func3, funct7
     * segments into the correct operation for the ALU.
     */
    ALUControl aluControl;
    /**
     * ALU responsible for performing arithmetic calculations.
     */
    ALU alu;

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
    /**
     * View the value currently stored in the specific register.
     */
    int32_t peekRegister(uint8_t registerNum) const;
};
