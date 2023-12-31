#include <cstdint>

#include "ALU.h"
#include "Controller.h"
#include "ImmGen.h"
#include "Instruction.h"
#include "MemUnit.h"
#include "RegFile.h"

class CPU
{
public:
    CPU();

    /**
     * Read the current value of the program counter (PC).
     */
    uint32_t readPC() const;

    /**
     * View the value currently stored in the specific register.
     */
    int32_t peekRegister(uint8_t registerNum) const;

    /**
     * Dump debugging information about the current state of the CPU to stderr.
     */
    void dumpState() const;

    /**
     * Top-level function that abstracts the fetch-decode-execute flow of one
     * CPU clock cycle. Return whether the CPU is still active i.e. `false` if
     * the program should terminate.
     */
    bool runCycle(uint8_t const *instructionMemory);

private:
    /**
     * Program counter.
     */
    uint32_t PC;
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
     * Fetch the current 32-bit instruction from the instruction memory.
     */
    uint32_t fetch(uint8_t const *instructionMemory);

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

    /**
     * Update the PC appropriately based on control flow.
     */
    void updatePC(int32_t immediate, int32_t writebackValue);
};
