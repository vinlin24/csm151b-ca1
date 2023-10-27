#include <bitset>
#include <iostream>

#include "Controller.h"

using namespace std;

Controller::Controller()
{
    this->clearSignals();
}

void Controller::clearSignals()
{
    for (int i = 0; i < ControllerSignals::NUM_SIGNALS; i++)
        this->signals[i] = false;
    this->aluOp = ALUOp::INVALID;
}

bool Controller::readSignal(ControllerSignals signalId) const
{
    return this->signals[signalId];
}

ALUOp Controller::readALUOp() const
{
    return this->aluOp;
}

void Controller::setSignals(bitset<7> opcode)
{
    this->clearSignals();

    using CS = ControllerSignals;
    switch (opcode.to_ulong())
    {
    case Opcodes::R_TYPE:
        this->signals[CS::RegWrite] = true;
        this->aluOp = ALUOp::FUNC;
        break;

    case Opcodes::I_TYPE:
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->aluOp = ALUOp::FUNC;
        break;

    case Opcodes::LW:
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->signals[CS::MemRead] = true;
        this->signals[CS::MemToReg] = true;
        this->aluOp = ALUOp::ADD;
        break;

    case Opcodes::SW:
        this->signals[CS::AluSrc] = true;
        this->signals[CS::MemWrite] = true;
        this->aluOp = ALUOp::ADD;
        break;

    case Opcodes::BLT:
        this->signals[CS::Branch] = true;
        this->aluOp = ALUOp::SUB;
        break;

    case Opcodes::JALR:
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->signals[CS::Link] = true;
        this->aluOp = ALUOp::ADD;
        break;

    // Something went wrong or the program terminated naturally.
    default:
        break;
    }
}
