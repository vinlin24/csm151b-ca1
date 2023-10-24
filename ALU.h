#ifndef ALU_H_INCLUDED
#define ALU_H_INCLUDED

#include <stdint.h>

enum ALUOperation
{
    ADD,
    SUB,
    XOR,
    SRA,
    AND,
};

class ALU
{
public:
    ALU();
    uint32_t compute(uint32_t input1, uint32_t input2, ALUOperation operation);
    bool readSignFlag() const;

private:
    bool signFlag;
};

#endif // ALU_H_INCLUDED
