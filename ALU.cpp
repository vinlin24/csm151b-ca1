#include "ALU.h"

#include <cassert>

ALU::ALU()
{
    this->signFlag = false;
}

int32_t ALU::compute(int32_t input1, int32_t input2, ALUOperation operation)
{
    int32_t result;
    switch (operation)
    {
    case ALU_ADD:
        result = input1 + input2;
        break;
    case ALU_SUB:
        result = input1 - input2;
        break;
    case ALU_XOR:
        result = input1 ^ input2;
        break;
    case ALU_SRA:
        result = input1 >> (input2 & 0b11111);
        break;
    case ALU_AND:
        result = input1 & input2;
        break;
    default:
        // ALU received unknown operation.
        assert(false);
    }
    this->signFlag = (result < 0);
    return result;
}

bool ALU::readSignFlag() const
{
    return this->signFlag;
}

ALUOperation
ALUControl::resolveOperation(ALUOp op, InstructionParts const &parts) const
{
    switch (op)
    {
    case ALUOp::ADD:
        return ALU_ADD;
    case ALUOp::SUB:
        return ALU_SUB;
    case ALUOp::FUNC:
        switch (parts.funct3.to_ulong())
        {
        case 0b000:
            if (parts.opcode == Opcodes::I_TYPE) // addi
                return ALU_ADD;
            if (parts.bit30)
                return ALU_SUB;
            return ALU_ADD;
        case 0b100:
            return ALU_XOR;
        case 0b101:
            return ALU_SRA;
        case 0b111:
            return ALU_AND;
        default:
            // Could not resolve operation with ALUOp=FUNC.
            assert(false);
        }
    default:
        // Received invalid ALUOp.
        assert(false);
    }
}
