#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
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

/*
 * This is the front end of your project. You need to first read the
 * instructions that are stored in a file and load them into an instruction
 * memory.
 */
int main(int argc, char *argv[])
{
    /*
     * Each cell should store 1 byte. You can define the memory either
     * dynamically, or define it as a fixed size with size 4KB (i.e., 4096
     * lines). Each instruction is 32 bits (i.e., 4 lines, saved in
     * little-endian mode). Each line in the input file is stored as an unsigned
     * char and is 1 byte (each four lines are one instruction). You need to
     * read the file line by line and store it into the memory. You may need a
     * mechanism to convert these values to bits so that you can read opcodes,
     * operands, etc.
     */

    bitset<8> instructionMemory[INSTRUCTION_MEMORY_SIZE];

    if (argc < 2)
    {
        cerr << "No file name entered. Exiting..." << endl;
        return EINVAL;
    }

    ifstream infile(argv[1]); // open the file
    if (!(infile.is_open() && infile.good()))
    {
        cerr << "error opening file" << endl;
        return EXIT_FAILURE;
    }

    string line;
    int i = 0;
    while (infile)
    {
        infile >> line;
        stringstream line2(line);
        int x;
        line2 >> x;
        instructionMemory[i] = bitset<8>(x);
        i++;
    }
    unsigned long maxPC = i;

    /*
     * Instantiate your CPU object here. CPU class is the main class in this
     * project that defines different components of the processor. CPU class
     * also has different functions for each stage (e.g., fetching an
     * instruction, decoding, etc.).
     */

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
