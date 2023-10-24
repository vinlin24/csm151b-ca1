#include <bitset>
#include <stdexcept>

#include "ALU.h"

using namespace std;

ALU::ALU()
{
    this->signFlag = false;
}

int32_t ALU::compute(int32_t input1, int32_t input2, ALUOperation operation)
{
    int32_t result;
    switch (operation)
    {
    case ALUOperation::ALU_ADD:
        result = input1 + input2;
        break;
    case ALUOperation::ALU_SUB:
        result = input1 - input2;
        break;
    case ALUOperation::ALU_XOR:
        result = input1 ^ input2;
        break;
    case ALUOperation::ALU_SRA:
        result = input1 >> (input2 & 0b11111);
        break;
    case ALUOperation::ALU_AND:
        result = input1 & input2;
        break;
    default:
        throw std::invalid_argument("ALU received unknown operation.");
    }
    this->signFlag = (result < 0);
    return result;
}

bool ALU::readSignFlag() const
{
    return this->signFlag;
}

ALUOperation
ALUControl::resolveOperation(ALUOp op, bool bit30, bitset<3> funct3) const
{
    switch (op)
    {
    case ALUOp::ADD:
        return ALUOperation::ALU_ADD;
    case ALUOp::SUB:
        return ALUOperation::ALU_SUB;
    case ALUOp::FUNC:
        switch (funct3.to_ulong())
        {
        case 0b000:
            return bit30 ? ALUOperation::ALU_SUB : ALUOperation::ALU_ADD;
        case 0b100:
            return ALUOperation::ALU_XOR;
        case 0b101:
            return ALUOperation::ALU_SRA;
        case 0b111:
            return ALUOperation::ALU_AND;
        default:
            throw std::invalid_argument(
                "Could not resolve operation with ALUOp=FUNC.");
        }
    default:
        throw std::invalid_argument("Received invalid ALUOp.");
    }
}
