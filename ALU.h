#ifndef ALU_H_INCLUDED
#define ALU_H_INCLUDED

#include <cstdint>

#include "Controller.h"
#include "Instruction.h"

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
    resolveOperation(ALUOp op, InstructionParts const &parts) const;
};

#endif // ALU_H_INCLUDED
