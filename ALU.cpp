#include "ALU.h"

ALU::ALU()
{
    this->signFlag = false;
}

uint32_t ALU::compute(uint32_t input1, uint32_t input2, ALUOperation operation)
{
    uint32_t result;
    switch (operation)
    {
    case ALUOperation::ADD:
        result = input1 + input2;
        break;
    case ALUOperation::SUB:
        result = input1 - input2;
        break;
    case ALUOperation::XOR:
        result = input1 ^ input2;
        break;
    case ALUOperation::SRA:
        result = static_cast<int32_t>(input1) >> (input2 & 0b11111);
        break;
    case ALUOperation::AND:
        result = input1 & input2;
        break;
    }
    int signBit = result & (1 << 31);
    this->signFlag = static_cast<bool>(signBit);
    return result;
}

bool ALU::readSignFlag() const
{
    return this->signFlag;
}
