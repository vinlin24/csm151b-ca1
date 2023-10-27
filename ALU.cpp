#include <bitset>
#include <stdexcept>
#include <iostream>

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
        throw invalid_argument("ALU received unknown operation.");
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
        return ALUOperation::ALU_ADD;
    case ALUOp::SUB:
        return ALUOperation::ALU_SUB;
    case ALUOp::FUNC:
        switch (parts.funct3.to_ulong())
        {
        case 0b000:
            if (parts.opcode == Opcodes::I_TYPE) // addi
            {
                // cerr << "Resolved ALUOperation from funct: ALU_ADD" << endl;
                return ALUOperation::ALU_ADD;
            }
            if (parts.bit30)
            {
                // cerr << "Resolved ALUOperation from funct: ALU_SUB" << endl;
                return ALUOperation::ALU_SUB;
            }
            // cerr << "Resolved ALUOperation from funct: ALU_ADD" << endl;
            return ALUOperation::ALU_ADD;
        case 0b100:
            // cerr << "Resolved ALUOperation from funct: ALU_XOR" << endl;
            return ALUOperation::ALU_XOR;
        case 0b101:
            // cerr << "Resolved ALUOperation from funct: ALU_SRA" << endl;
            return ALUOperation::ALU_SRA;
        case 0b111:
            // cerr << "Resolved ALUOperation from funct: ALU_AND" << endl;
            return ALUOperation::ALU_AND;
        default:
            throw invalid_argument(
                "Could not resolve operation with ALUOp=FUNC.");
        }
    default:
        throw invalid_argument("Received invalid ALUOp.");
    }
}
