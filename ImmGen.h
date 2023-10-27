#ifndef IMMGEN_H_INCLUDED
#define IMMGEN_H_INCLUDED

#include <stdint.h>

#include "Controller.h"

class ImmGen
{
public:
    int32_t generate(uint32_t instruction, Opcodes opcode) const;
};

#endif // IMMGEN_H_INCLUDED
