#include <bitset>

#include "Controller.h"

using namespace std;

namespace Opcodes
{
    enum Opcodes_
    {
        R_TYPE = 0b0110011,
        I_TYPE = 0b0010011,
        LW = 0b0000011,
        SW = 0b0100011,
        BLT = 0b1100011,
        JALR = 0b1100111,
    };
}

Controller::Controller()
{
    this->flags = static_cast<ControllerFlags>(0);
    this->op = ALUOp::INVALID;
}

void Controller::setSignals(bitset<7> opcode)
{
    using CF = ControllerFlags;
    unsigned long bitPattern = opcode.to_ulong();

    switch (bitPattern)
    {
    case Opcodes::R_TYPE:
        this->flags = CF::RegWrite;
        this->op = ALUOp::FUNC;
        break;

    case Opcodes::I_TYPE:
        this->flags = CF::RegWrite | CF::AluSrc;
        this->op = ALUOp::FUNC;
        break;

    case Opcodes::LW:
        this->flags = CF::RegWrite | CF::AluSrc | CF::MemRead | CF::MemToReg;
        this->op = ALUOp::ADD;
        break;

    case Opcodes::SW:
        this->flags = CF::AluSrc | CF::MemWrite;
        this->op = ALUOp::ADD;
        break;

    case Opcodes::BLT:
        this->flags = CF::Branch;
        this->op = ALUOp::SUB;
        break;

    case Opcodes::JALR:
        this->flags = CF::RegWrite | CF::AluSrc | CF::Link;
        this->op = ALUOp::ADD;
        break;

    // Something went wrong.
    default:
        this->flags = static_cast<ControllerFlags>(0);
        this->op = ALUOp::INVALID;
    }
}
