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
        cerr << "Controller::setSignals: instruction type is R-Type" << endl;
        this->signals[CS::RegWrite] = true;
        this->aluOp = ALUOp::FUNC;
        break;

    case Opcodes::I_TYPE:
        cerr << "Controller::setSignals: instruction type is I-Type" << endl;
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->aluOp = ALUOp::FUNC;
        break;

    case Opcodes::LW:
        cerr << "Controller::setSignals: instruction type is LW" << endl;
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->signals[CS::MemRead] = true;
        this->signals[CS::MemToReg] = true;
        this->aluOp = ALUOp::ADD;
        break;

    case Opcodes::SW:
        cerr << "Controller::setSignals: instruction type is SW" << endl;
        this->signals[CS::AluSrc] = true;
        this->signals[CS::MemWrite] = true;
        this->aluOp = ALUOp::ADD;
        break;

    case Opcodes::BLT:
        cerr << "Controller::setSignals: instruction type is BLT" << endl;
        this->signals[CS::Branch] = true;
        this->aluOp = ALUOp::SUB;
        break;

    case Opcodes::JALR:
        cerr << "Controller::setSignals: instruction type is JALR" << endl;
        this->signals[CS::RegWrite] = true;
        this->signals[CS::AluSrc] = true;
        this->signals[CS::Link] = true;
        this->aluOp = ALUOp::ADD;
        break;

    // Something went wrong.
    default:
        cerr << "Controller::setSignals: instruction type is invalid" << endl;
    }
}
