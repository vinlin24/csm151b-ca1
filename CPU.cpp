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
    bitset<3> funct3((bits & (0b111 << 12)) >> 12);
    bool bit30 = (bits & (1 << 30));
    cerr << "CPU::decode: opcode, bits[6:0] is: "
         << opcode.to_string() << endl;
    cerr << "CPU::decode: rs1, bits[19:15] is: "
         << rs1.to_string() << endl;
    cerr << "CPU::decode: rs2, bits[24:20] is: "
         << rs2.to_string() << endl;
    cerr << "CPU::decode: rd, bits[11:7] is: "
         << rd.to_string() << endl;
    cerr << "CPU::decode: funct3, bits[14:12] is: "
         << funct3.to_string() << endl;
    cerr << "CPU::decode: bit30, bits[30] is: "
         << (bit30 ? "1" : "0") << endl;

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

    ALUOp aluOp = this->controller.readALUOp();
    cerr << "CPU::decode: ALUOp is: "
         << static_cast<int>(aluOp) << endl;

    // Terminate the moment we encounter an invalid instruction. The #end
    // "instruction" will result in this being hit, so this is also how we
    // terminate normally.
    if (aluOp == ALUOp::INVALID)
        return false;

    // Execute. TODO: move this to own method(s)?

    Opcodes opcodeBits = static_cast<Opcodes>(opcode.to_ulong());
    int32_t immediate = this->immGen.generate(bits, opcodeBits);

    int32_t rs1Data = this->regFile.readRegister(rs1.to_ulong());
    int32_t rs2Data = this->regFile.readRegister(rs2.to_ulong());

    ALUOperation aluOperation = this->aluControl.resolveOperation(
        aluOp, bit30, funct3);

    int32_t aluOutput;
    if (this->controller.readSignal(CS::AluSrc))
        aluOutput = this->alu.compute(rs1Data, immediate, aluOperation);
    else
        aluOutput = this->alu.compute(rs1Data, rs2Data, aluOperation);

    // TODO: do something with signFlag.
    bool signFlag = this->alu.readSignFlag();
    cerr << "CPU::decode: aluOutput = " << aluOutput
         << " (signFlag=" << (signFlag ? "1)" : "0)") << endl;

    int32_t writebackValue;
    if (this->controller.readSignal(CS::MemRead))
        writebackValue = this->memUnit.readData(aluOutput);
    else
        writebackValue = aluOutput;

    if (this->controller.readSignal(CS::MemWrite))
        this->memUnit.writeData(aluOutput, rs2Data);

    if (this->controller.readSignal(CS::RegWrite))
        this->regFile.writeRegister(rd.to_ulong(), writebackValue);

    return true;
}

unsigned long CPU::readPC()
{
    return this->PC;
}

int32_t CPU::peekRegister(uint8_t registerNum) const
{
    return this->regFile.readRegister(registerNum);
}
