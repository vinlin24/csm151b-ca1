#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class instruction
{
public:
    std::bitset<32> instr;              // instruction
    instruction(std::bitset<32> fetch); // constructor
};

class CPU
{
private:
    int dmemory[4096]; // data memory byte addressable in little endian fashion;
    unsigned long PC;  // pc

public:
    CPU();
    unsigned long readPC();
    std::bitset<32> Fetch(std::bitset<8> *instmem);
    bool Decode(instruction *instr);
};
