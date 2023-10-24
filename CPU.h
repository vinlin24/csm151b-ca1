#include <bitset>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "ALU.h"
#include "Controller.h"
#include "ImmGen.h"
#include "MemUnit.h"
#include "RegFile.h"

class CPU
{
public:
    CPU();
    /**
     * Read the current value of the program counter (PC).
     */
    unsigned long readPC();

    /**
     * Top-level function that abstracts the fetch-decode-execute flow of one
     * CPU clock cycle. Returns whether the CPU is still active i.e. `false` if
     * the program should terminate.
     */
    bool runCycle(std::bitset<8> *instructionMemory);

    /**
     * View the value currently stored in the specific register.
     */
    int32_t peekRegister(uint8_t registerNum) const;

private:
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
    RegFile regFile;
    /**
     * Immediate generator unit responsible for extracting and sign extending
     * immediates encoded in the instruction.
     */
    ImmGen immGen;
    /**
     * Unit responsible for resolving the ALUOp control signal and func3, funct7
     * segments into the correct operation for the ALU.
     */
    ALUControl aluControl;
    /**
     * ALU responsible for performing arithmetic calculations.
     */
    ALU alu;
    /**
     * Unit responsible for keeping track of the data memory available to CPU,
     * with support for read/write operations.
     */
    MemUnit memUnit;

    /**
     * Parts extracted from an instruction. Each instruction may or may not use
     * all of them.
     */
    struct InstructionParts
    {
        std::bitset<7> opcode;
        std::bitset<5> rs1;
        std::bitset<5> rs2;
        std::bitset<5> rd;
        std::bitset<3> funct3;
        bool bit30;
        int32_t immediate;
    };

    /**
     * Fetch the current 32-bit instruction from the instruction memory.
     */
    uint32_t fetch(std::bitset<8> *instructionMemory);

    /**
     * Decode the given instruction by setting the appropriate controller
     * signals and returning the parsed parts of the instruction.
     */
    InstructionParts decode(uint32_t instruction);

    /**
     * Execute the instruction and returns whether the CPU is still active i.e.
     * `false` if the program should terminate.
     */
    bool execute(InstructionParts const &parts);
};
