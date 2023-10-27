#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "CPU.h"

using namespace std;

#define INSTRUCTION_MEMORY_SIZE 4096

/**
 * a0 is the symbolic name for register x10.
 */
#define REG_A0 10
/**
 * a1 is the symbolic name for register x11.
 */
#define REG_A1 11

/**
 * Initialize the instruction memory array from the contents of a byte stream in
 * the instMem format.
 */
static uint32_t initializeInstructionMemory(ifstream &infile,
                                            uint8_t *instructionMemory)
{
    string line;
    int i = 0;
    while (infile)
    {
        infile >> line;
        stringstream line2(line);
        int x;
        line2 >> x;
        instructionMemory[i] = x;
        i++;
    }
    uint32_t maxPC = i;
    return maxPC;
}

/*
 * This is the front end of your project. You need to first read the
 * instructions that are stored in a file and load them into an instruction
 * memory.
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "No file name entered. Exiting..." << endl;
        return EINVAL;
    }

    ifstream infile(argv[1]);
    if (!(infile.is_open() && infile.good()))
    {
        cerr << "Error opening file. Exiting..." << endl;
        return EXIT_FAILURE;
    }

    uint8_t instructionMemory[INSTRUCTION_MEMORY_SIZE];
    uint32_t maxPC = initializeInstructionMemory(infile, instructionMemory);

    CPU cpu;
    int32_t a0Value = 0;
    int32_t a1Value = 0;

    // For debugging.
    cpu.dumpState();
    int cyclesExecuted = 0;

    // Processor's main loop. Each iteration is equal to one clock cycle.
    while (cpu.runCycle(instructionMemory))
    {
        // Query the registers to test.
        a0Value = cpu.peekRegister(REG_A0);
        a1Value = cpu.peekRegister(REG_A1);

        // For debugging.
        cpu.dumpState();
        cyclesExecuted++;

        // Sanity check.
        if (cpu.readPC() > maxPC)
            break;
    }

    // Final one-line output, formatted the exact way described in the spec.
    cout << "(" << a0Value << "," << a1Value << ")" << endl;

    cerr << "(Executed " << cyclesExecuted << " cycles)" << endl;

    return EXIT_SUCCESS;
}
