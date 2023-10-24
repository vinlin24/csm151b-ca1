#ifndef IMMGEN_H_INCLUDED
#define IMMGEN_H_INCLUDED

#include <bitset>
#include <stdint.h>

#include "Controller.h"

class ImmGen
{
public:
    int32_t generate(uint32_t instruction, Opcodes opcode);
};

#endif // IMMGEN_H_INCLUDED
