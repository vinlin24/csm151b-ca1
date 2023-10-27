#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <bitset>

enum ALUOp
{
    INVALID, // Some default for debugging I guess.
    FUNC,
    ADD,
    SUB,
};

enum Opcodes
{
    R_TYPE = 0b0110011,
    I_TYPE = 0b0010011,
    LW = 0b0000011,
    SW = 0b0100011,
    BLT = 0b1100011,
    JALR = 0b1100111,
};

enum ControllerSignals
{
    RegWrite,
    AluSrc,
    Branch,
    MemRead,
    MemWrite,
    MemToReg,
    Link,
    NUM_SIGNALS,
};

class Controller
{
public:
    Controller();

    bool readSignal(ControllerSignals signalId) const;
    ALUOp readALUOp() const;

    void setSignals(std::bitset<7> opcode);

private:
    bool signals[ControllerSignals::NUM_SIGNALS];
    ALUOp aluOp;

    void clearSignals();
};

#endif // CONTROLLER_H_INCLUDED
