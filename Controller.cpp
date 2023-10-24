#include <bitset>
#include <iostream>

#include "Controller.h"

using namespace std;

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
        cerr << "Controller::setSignals: instruction type is R-Type" << endl;
        this->flags = CF::RegWrite;
        this->op = ALUOp::FUNC;
        break;

    case Opcodes::I_TYPE:
        cerr << "Controller::setSignals: instruction type is I-Type" << endl;
        this->flags = CF::RegWrite | CF::AluSrc;
        this->op = ALUOp::FUNC;
        break;

    case Opcodes::LW:
        cerr << "Controller::setSignals: instruction type is LW" << endl;
        this->flags = CF::RegWrite | CF::AluSrc | CF::MemRead | CF::MemToReg;
        this->op = ALUOp::ADD;
        break;

    case Opcodes::SW:
        cerr << "Controller::setSignals: instruction type is SW" << endl;
        this->flags = CF::AluSrc | CF::MemWrite;
        this->op = ALUOp::ADD;
        break;

    case Opcodes::BLT:
        cerr << "Controller::setSignals: instruction type is BLT" << endl;
        this->flags = CF::Branch;
        this->op = ALUOp::SUB;
        break;

    case Opcodes::JALR:
        cerr << "Controller::setSignals: instruction type is JALR" << endl;
        this->flags = CF::RegWrite | CF::AluSrc | CF::Link;
        this->op = ALUOp::ADD;
        break;

    // Something went wrong.
    default:
        cerr << "Controller::setSignals: instruction type is invalid" << endl;
        this->flags = static_cast<ControllerFlags>(0);
        this->op = ALUOp::INVALID;
    }
}
