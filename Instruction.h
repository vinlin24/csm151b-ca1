#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <bitset>
#include <cstdint>

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

#endif // INSTRUCTION_H_INCLUDED
