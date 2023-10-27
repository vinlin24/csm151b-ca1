#include "MemUnit.h"

#include <iostream>
#include <stddef.h>

using namespace std;

MemUnit::MemUnit()
{
    for (size_t i = 0; i < DATA_MEMORY_SIZE; i++)
        this->dataMemory[i] = 0;
}

int32_t MemUnit::readData(uint32_t address) const
{
    int32_t word = this->dataMemory[address / 4];
    return word;
}

void MemUnit::writeData(uint32_t address, int32_t data)
{
    this->dataMemory[address / 4] = data;
}
