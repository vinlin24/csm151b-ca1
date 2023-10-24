#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <bitset>

enum class ALUOp
{
    INVALID, // Some default for debugging I guess.
    FUNC,
    ADD,
    SUB,
};

enum class ControllerFlags
{
    RegWrite = (1 << 0),
    AluSrc = (1 << 1),
    Branch = (1 << 2),
    MemRead = (1 << 3),
    MemWrite = (1 << 4),
    MemToReg = (1 << 5),
    Link = (1 << 6), // Custom flag for the JALR instruction.
};

static inline ControllerFlags operator|(ControllerFlags a, ControllerFlags b)
{
    return static_cast<ControllerFlags>(
        static_cast<int>(a) | static_cast<int>(b));
}

static inline ControllerFlags operator&(ControllerFlags a, ControllerFlags b)
{
    return static_cast<ControllerFlags>(
        static_cast<int>(a) & static_cast<int>(b));
}

class Controller
{
public:
    Controller();
    void setSignals(std::bitset<7> opcode);
    ControllerFlags readFlags() const;
    ALUOp readALUOp() const;

private:
    ControllerFlags flags;
    ALUOp op;
};

inline ControllerFlags Controller::readFlags() const
{
    return this->flags;
}

inline ALUOp Controller::readALUOp() const
{
    return this->op;
}

#endif // CONTROLLER_H_INCLUDED
