#include "CPU.h"

using namespace std;

Instruction::Instruction() {}

Instruction::Instruction(bitset<32> fetch)
{
    this->instruction = fetch;
}

CPU::CPU()
{
    this->PC = 0;
    for (int i = 0; i < 4096; i++)
    {
        this->dataMemory[i] = (0);
    }
}

bitset<32> CPU::fetch(bitset<8> *instructionMemory)
{
    // get 32 bit instruction
    bitset<32> instruction = ((((instructionMemory[PC + 3].to_ulong()) << 24)) +
                              ((instructionMemory[PC + 2].to_ulong()) << 16) +
                              ((instructionMemory[PC + 1].to_ulong()) << 8) +
                              (instructionMemory[PC + 0].to_ulong()));
    cerr << "\nCPU::fetch: PC=" << this->PC << endl;
    this->PC += 4;
    return instruction;
}

/**
 * Helper function to format the output of the controller flags as a binary
 * string. For debugging purposes only.
 */
static string spaceOutFlagBits(ControllerFlags cf)
{
    string result = "|";
    bitset<7> bits(static_cast<int>(cf));
    for (int i = 0; i < 7; i++)
    {
        result += bits[i] ? "1 |" : "0 |";
    }
    return result;
}

bool CPU::decode(Instruction *current)
{
    bitset<32> bits = current->instruction;
    cerr << "CPU::decode: received instruction: " << bits.to_string() << endl;
    bitset<7> opcode(bits.to_ulong() & 0x7F);
    cerr << "CPU::decode: opcode, bits[6:0] is: " << opcode.to_string() << endl;

    this->controller.setSignals(opcode);
    ControllerFlags flags = this->controller.readFlags();
    ALUOp op = this->controller.readALUOp();

    cerr << "CPU::decode: controller flags ref: "
         << "|RW|AS|Br|MR|MW|tR|Ln|" << endl;
    cerr << "CPU::decode: controller flags are: "
         << spaceOutFlagBits(flags) << endl;
    cerr << "CPU::decode: ALUOp is: "
         << static_cast<int>(op) << endl;

    // Terminate the moment we encounter an invalid instruction. The #end
    // "instruction" will result in this being hit, so this is also how we
    // terminate normally.
    return op != ALUOp::INVALID;
}

unsigned long CPU::readPC()
{
    return this->PC;
}
