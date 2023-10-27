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
    for (uint32_t address = 0; address < DATA_MEMORY_SIZE; address += 4)
    {
        int32_t data = this->memUnit.readData(address);
        if (data != 0)
            cerr << "0x" << hex << address << dec << ": " << data << endl;
    }
}

bool CPU::runCycle(uint8_t const *instructionMemory)
{
    uint32_t instruction = this->fetch(instructionMemory);
    InstructionParts parts = this->decode(instruction);
    bool stillRunning = this->execute(parts);
    return stillRunning;
}

uint32_t CPU::fetch(uint8_t const *instructionMemory)
{
    uint32_t instruction = (((instructionMemory[PC + 3]) << 24)) +
                           ((instructionMemory[PC + 2]) << 16) +
                           ((instructionMemory[PC + 1]) << 8) +
                           (instructionMemory[PC + 0]);
    return instruction;
}

InstructionParts CPU::decode(uint32_t instruction)
{
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

    this->controller.setSignals(parts.opcode);
    return parts;
}

bool CPU::execute(InstructionParts const &parts)
{
    using CS = ControllerSignals;

    // Terminate the moment we encounter an invalid instruction. The #end
    // "instruction" will result in this being hit, so this is also how we
    // terminate normally.

    ALUOp aluOp = this->controller.readALUOp();
    if (aluOp == ALUOp::INVALID)
        return false;

    // RegFile: Read register data, regardless if they will be used this cycle.

    int32_t rs1Data = this->regFile.readRegister(parts.rs1.to_ulong());
    int32_t rs2Data = this->regFile.readRegister(parts.rs2.to_ulong());

    // ALU: Determine the operation & operands and execute.

    ALUOperation aluOperation = this->aluControl.resolveOperation(aluOp, parts);
    int32_t aluOutput;
    if (this->controller.readSignal(CS::AluSrc))
        aluOutput = this->alu.compute(rs1Data, parts.immediate, aluOperation);
    else
        aluOutput = this->alu.compute(rs1Data, rs2Data, aluOperation);

    // Memory: Write to memory if applicable.

    if (this->controller.readSignal(CS::MemWrite))
        this->memUnit.writeData(aluOutput, rs2Data);

    // Writeback: Determine what value to potentially write back to the
    // destination register.

    int32_t writebackValue;
    if (this->controller.readSignal(CS::MemRead))
        writebackValue = this->memUnit.readData(aluOutput);
    else
        writebackValue = aluOutput;

    // Writeback: Write back to the destination register if applicable.

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

    // This is a link instruction, so we should set the PC to what we computed.
    if (this->controller.readSignal(CS::Link))
    {
        this->PC = static_cast<uint32_t>(writebackValue & ~1);
    }
    // This is a branch instruction, so we should add an offset to the PC.
    else if (this->controller.readSignal(CS::Branch) && signFlag)
    {
        uint32_t offset = static_cast<uint32_t>(immediate << 1);
        this->PC += offset;
    }
    // Otherwise simply step to the next instruction in memory.
    else
    {
        this->PC += 4;
    }
}
