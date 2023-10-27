#include "CPU.h"

#include <iostream>

using namespace std;

CPU::CPU()
{
    this->PC = 0;
}

uint32_t CPU::readPC() const
{
    return this->PC;
}

int32_t CPU::peekRegister(uint8_t registerNum) const
{
    return this->regFile.readRegister(registerNum);
}

void CPU::dumpState() const
{
    cerr << "========== New PC: 0x" << hex << this->PC << dec
         << " ==========" << endl;

    cerr << "[REGISTERS]" << endl;
    for (uint8_t id = 0; id < NUM_REGISTERS; id++)
    {
        int32_t value = this->peekRegister(id);
        if (value != 0)
            cerr << "x" << static_cast<int>(id) << ": " << value << endl;
    }

    cerr << "[MEMORY]" << endl;
    for (uint32_t word = 0; word < DATA_MEMORY_SIZE; word++)
    {
        uint32_t address = word * 4;
        int32_t data = this->memUnit.readData(address);
        if (data != 0)
            cerr << "0x" << hex << address << dec << ": " << data << endl;
    }
}

bool CPU::runCycle(bitset<8> const *instructionMemory)
{
    uint32_t instruction = this->fetch(instructionMemory);
    InstructionParts parts = this->decode(instruction);
    bool stillRunning = this->execute(parts);
    return stillRunning;
}

uint32_t CPU::fetch(bitset<8> const *instructionMemory)
{
    uint32_t instruction = (((instructionMemory[PC + 3].to_ulong()) << 24)) +
                           ((instructionMemory[PC + 2].to_ulong()) << 16) +
                           ((instructionMemory[PC + 1].to_ulong()) << 8) +
                           (instructionMemory[PC + 0].to_ulong());
    // cerr << "\nCPU::fetch: PC=" << this->PC << endl;
    return instruction;
}

InstructionParts CPU::decode(uint32_t instruction)
{
    // cerr << "CPU::decode: received instruction: "
    //      << bitset<32>(instruction).to_string() << endl;

    // Split up the instruction as illustrated in the datapath design.
    InstructionParts parts;
    parts.opcode = (instruction & 0b1111111);
    parts.rs1 = ((instruction & (0b11111 << 15)) >> 15);
    parts.rs2 = ((instruction & (0b11111 << 20)) >> 20);
    parts.rd = ((instruction & (0b11111 << 7)) >> 7);
    parts.funct3 = ((instruction & (0b111 << 12)) >> 12);
    parts.bit30 = (instruction & (1 << 30));

    // Also decode the immediate.
    Opcodes opcodeBits = static_cast<Opcodes>(parts.opcode.to_ulong());
    parts.immediate = this->immGen.generate(instruction, opcodeBits);

    // cerr << "CPU::decode: opcode, bits[6:0] is: "
    //      << parts.opcode.to_string() << endl;
    // cerr << "CPU::decode: rs1, bits[19:15] is: "
    //      << parts.rs1.to_string() << endl;
    // cerr << "CPU::decode: rs2, bits[24:20] is: "
    //      << parts.rs2.to_string() << endl;
    // cerr << "CPU::decode: rd, bits[11:7] is: "
    //      << parts.rd.to_string() << endl;
    // cerr << "CPU::decode: funct3, bits[14:12] is: "
    //      << parts.funct3.to_string() << endl;
    // cerr << "CPU::decode: bit30, bits[30] is: "
    //      << (parts.bit30 ? "1" : "0") << endl;
    // cerr << "CPU::decode: immediate is: "
    //      << parts.immediate << endl;

    this->controller.setSignals(parts.opcode);
    return parts;
}

bool CPU::execute(InstructionParts const &parts)
{
    using CS = ControllerSignals;
    // cerr << "CPU::execute: controller flags ref: |RW|AS|Br|MR|MW|tR|Ln|"
    //      << endl;
    // cerr << "CPU::execute: controller flags are: |";
    // for (int signalId = 0; signalId < CS::NUM_SIGNALS; signalId++)
    // {
    //     bool signal = this->controller.readSignal(static_cast<CS>(signalId));
    //     cerr << (signal ? "1 |" : "0 |");
    // }
    // cerr << endl;

    ALUOp aluOp = this->controller.readALUOp();
    // cerr << "CPU::execute: ALUOp is: " << static_cast<int>(aluOp) << endl;

    // Terminate the moment we encounter an invalid instruction. The #end
    // "instruction" will result in this being hit, so this is also how we
    // terminate normally.
    if (aluOp == ALUOp::INVALID)
        return false;

    int32_t rs1Data = this->regFile.readRegister(parts.rs1.to_ulong());
    int32_t rs2Data = this->regFile.readRegister(parts.rs2.to_ulong());

    ALUOperation aluOperation = this->aluControl.resolveOperation(aluOp, parts);

    int32_t aluOutput;
    if (this->controller.readSignal(CS::AluSrc))
        aluOutput = this->alu.compute(rs1Data, parts.immediate, aluOperation);
    else
        aluOutput = this->alu.compute(rs1Data, rs2Data, aluOperation);

    // bool signFlag = this->alu.readSignFlag();
    // cerr << "CPU::execute: aluOutput = " << aluOutput
    //      << " (signFlag=" << (signFlag ? "1)" : "0)") << endl;

    // Determine what value to potentially write back to the register file.
    int32_t writebackValue;
    if (this->controller.readSignal(CS::MemRead))
        writebackValue = this->memUnit.readData(aluOutput);
    else
        writebackValue = aluOutput;

    if (this->controller.readSignal(CS::MemWrite))
        this->memUnit.writeData(aluOutput, rs2Data);

    if (this->controller.readSignal(CS::RegWrite))
    {
        int32_t valueToWrite;
        if (this->controller.readSignal(CS::Link))
            valueToWrite = this->PC + 4;
        else
            valueToWrite = writebackValue;
        this->regFile.writeRegister(parts.rd.to_ulong(), valueToWrite);
    }

    this->updatePC(parts.immediate, writebackValue);
    return true;
}

void CPU::updatePC(int32_t immediate, int32_t writebackValue)
{
    using CS = ControllerSignals;
    bool signFlag = this->alu.readSignFlag();

    if (this->controller.readSignal(CS::Link))
    {
        this->PC = static_cast<uint32_t>(writebackValue & ~1);
        // cerr << "CPU::updatePC: linked to address " << this->PC << endl;
    }
    else if (this->controller.readSignal(CS::Branch) && signFlag)
    {
        uint32_t offset = static_cast<uint32_t>(immediate << 1);
        this->PC += offset;
        // cerr << "CPU::updatePC: jumped by " << offset << " to address "
        //      << this->PC << endl;
    }
    else
    {
        this->PC += 4;
    }
}
