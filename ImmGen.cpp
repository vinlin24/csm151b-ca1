#include <bitset>

#include "ImmGen.h"

using namespace std;

/**
 * imm[11:0] encoded in instruction[31:20].
 */
static int32_t generateUsual(uint32_t instruction)
{
    // Extract.
    bitset<12> extracted((instruction & (0xFFF << 20)) >> 20);
    int32_t immediate = extracted.to_ulong();
    // Sign extend.
    int shift = 32 - extracted.size();
    immediate <<= shift;
    immediate >>= shift;
    return immediate;
}

/**
 * imm[11:5] encoded in instruction[31:25], imm[4:0] in instruction[11:7].
 */
static int32_t generateForSW(uint32_t instruction)
{
    // Extract.
    bitset<6> upper((instruction & (0x7F << 25)) >> 25);
    bitset<5> lower((instruction & (0x1F << 7)) >> 7);
    // Concatenate.
    int32_t immediate = 0;
    immediate |= (upper.to_ulong() << lower.size());
    immediate |= lower.to_ulong();
    // Sign extend.
    int shift = 32 - (upper.size() + lower.size());
    immediate <<= shift;
    immediate >>= shift;
    return immediate;
}

/**
 * imm[12] encoded in instruction[31], imm[10:5] in instruction[30:25], imm[4:1]
 * in instruction[11:8], imm[11] in instruction[7].
 */
static int32_t generateForBLT(uint32_t instruction)
{
    // Extract.
    bool bit12 = instruction & (1 << 31);
    bool bit11 = instruction & (1 << 7);
    bitset<6> upper((instruction & (0x3F << 25)) >> 25);
    bitset<4> lower((instruction & (0xF << 8)) >> 8);
    // Concatenate.
    int32_t immediate = 0;
    immediate |= (bit12 << 12);
    immediate |= (bit11 << 11);
    immediate |= (upper.to_ulong() << lower.size());
    immediate |= lower.to_ulong();
    // Sign extend.
    int shift = 32 - 12;
    immediate <<= shift;
    immediate >>= shift;
    return immediate;
}

int32_t ImmGen::generate(uint32_t instruction, Opcodes opcode)
{
    switch (opcode)
    {

    case Opcodes::I_TYPE:
    case Opcodes::LW:
    case Opcodes::JALR:
        return generateUsual(instruction);
    case Opcodes::SW:
        return generateForSW(instruction);
    case Opcodes::BLT:
        return generateForBLT(instruction);
    // The instruction doesn't use an immediate, so just return garbage.
    default:
        return 69420;
    }
}
