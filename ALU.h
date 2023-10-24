#ifndef ALU_H_INCLUDED
#define ALU_H_INCLUDED

#include <bitset>
#include <stdint.h>

#include "Controller.h"

enum ALUOperation
{
    ALU_ADD,
    ALU_SUB,
    ALU_XOR,
    ALU_SRA,
    ALU_AND,
};

class ALU
{
public:
    ALU();
    int32_t compute(int32_t input1, int32_t input2, ALUOperation operation);
    bool readSignFlag() const;

private:
    bool signFlag;
};

class ALUControl
{
public:
    ALUOperation
    resolveOperation(ALUOp op, bool bit30, std::bitset<3> funct3) const;
};

#endif // ALU_H_INCLUDED
