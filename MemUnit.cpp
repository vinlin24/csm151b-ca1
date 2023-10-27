#include "MemUnit.h"

int32_t fromLittleEndian(uint8_t const *memoryAddress)
{
    uint8_t byte0 = memoryAddress[0];
    uint8_t byte1 = memoryAddress[1];
    uint8_t byte2 = memoryAddress[2];
    uint8_t byte3 = memoryAddress[3];

    int32_t word = 0;
    word |= byte0;
    word |= (byte1 << 8);
    word |= (byte2 << 16);
    word |= (byte3 << 24);
    return word;
}

void toLittleEndian(int32_t data, uint8_t *memoryAddress)
{
    uint8_t byte0 = data & 0xFF;
    uint8_t byte1 = (data & (0xFF << 8)) >> 8;
    uint8_t byte2 = (data & (0xFF << 16)) >> 16;
    uint8_t byte3 = (data & (0xFF << 24)) >> 24;

    memoryAddress[0] = byte0;
    memoryAddress[1] = byte1;
    memoryAddress[2] = byte2;
    memoryAddress[3] = byte3;
}

MemUnit::MemUnit()
{
    for (uint32_t i = 0; i < DATA_MEMORY_SIZE; i++)
        this->dataMemory[i] = 0;
}

int32_t MemUnit::readData(uint32_t address) const
{
    int32_t word = fromLittleEndian(&this->dataMemory[address]);
    return word;
}

void MemUnit::writeData(uint32_t address, int32_t data)
{
    toLittleEndian(data, &this->dataMemory[address]);
}
