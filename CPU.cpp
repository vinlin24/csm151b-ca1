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

bool CPU::decode(Instruction *current)
{
    cerr << "CPU::decode: received instruction: "
         << current->instruction.to_string() << endl;

    // Split up the instruction as illustrated in the datapath design.
    uint32_t bits = current->instruction.to_ulong();
    bitset<7> opcode(bits & 0b1111111);
    bitset<5> rs1((bits & (0b11111 << 15)) >> 15);
    bitset<5> rs2((bits & (0b11111 << 20)) >> 20);
    bitset<5> rd((bits & (0b11111 << 7)) >> 7);
    cerr << "CPU::decode: opcode, bits[6:0] is: " << opcode.to_string() << endl;
    cerr << "CPU::decode: rs1, bits[19:15] is: " << rs1.to_string() << endl;
    cerr << "CPU::decode: rs2, bits[24:20] is: " << rs2.to_string() << endl;
    cerr << "CPU::decode: rd, bits[11:7] is: " << rd.to_string() << endl;

    this->controller.setSignals(opcode);

    cerr << "CPU::decode: controller flags ref: "
         << "|RW|AS|Br|MR|MW|tR|Ln|" << endl;
    cerr << "CPU::decode: controller flags are: |";

    using CS = ControllerSignals;
    for (int signalId = 0; signalId < CS::NUM_SIGNALS; signalId++)
    {
        bool signal = this->controller.readSignal(static_cast<CS>(signalId));
        cerr << (signal ? "1 |" : "0 |");
    }
    cerr << endl;

    ALUOp op = this->controller.readALUOp();
    cerr << "CPU::decode: ALUOp is: "
         << static_cast<int>(op) << endl;

    // Terminate the moment we encounter an invalid instruction. The #end
    // "instruction" will result in this being hit, so this is also how we
    // terminate normally.
    if (op == ALUOp::INVALID)
        return false;

    // TODO?

    return true;
}

unsigned long CPU::readPC()
{
    return this->PC;
}
