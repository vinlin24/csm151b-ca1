#include "CPU.h"

#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

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

    bitset<8> instructionMemory[4096];

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
    int maxPC = i;

    /*
     * Instantiate your CPU object here. CPU class is the main class in this
     * project that defines different components of the processor. CPU class
     * also has different functions for each stage (e.g., fetching an
     * instruction, decoding, etc.).
     */

    // call the appropriate constructor here to initialize the processor...
    CPU cpu;
    // make sure to create a variable for PC and resets it to zero (e.g.,
    // unsigned int PC = 0);

    /* OPTIONAL: Instantiate your Instruction object here. */

    bitset<32> current;
    Instruction instruction;

    // processor's main loop. Each iteration is equal to one clock cycle.
    bool active = true;
    while (active)
    {
        // fetch
        current = cpu.fetch(instructionMemory); // fetching the instruction
        instruction = Instruction(current);

        // decode
        active = cpu.decode(&instruction);

        // break from loop so stats are not mistakenly updated
        if (!active)
            break;
        // the rest should be implemented here ...
        // ...

        // sanity check
        if (cpu.readPC() > maxPC)
            break;
    }

    int a0 = 0;
    int a1 = 0;
    // TODO: print the results (you should replace a0 and a1 with your own
    // variables that point to a0 and a1)
    cout << "(" << a0 << "," << a1 << ")" << endl;

    return EXIT_SUCCESS;
}
