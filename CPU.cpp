#include "CPU.h"

using namespace std;

Instruction::Instruction() {}

Instruction::Instruction(bitset<32> fetch)
{
    this->instruction = fetch;
}

CPU::CPU()
{
    this->PC = 0;
    for (int i = 0; i < 4096; i++)
    {
        this->dataMemory[i] = (0);
    }
}

bitset<32> CPU::fetch(bitset<8> *instructionMemory)
{
    // get 32 bit instruction
    bitset<32> instruction = ((((instructionMemory[PC + 3].to_ulong()) << 24)) +
                              ((instructionMemory[PC + 2].to_ulong()) << 16) +
                              ((instructionMemory[PC + 1].to_ulong()) << 8) +
                              (instructionMemory[PC + 0].to_ulong()));
    this->PC += 4;
    return instruction;
}

bool CPU::decode(Instruction *current)
{
    return true; // TODO.
}

unsigned long CPU::readPC()
{
    return this->PC;
}
