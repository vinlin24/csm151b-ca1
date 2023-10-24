#include "RegFile.h"

RegFile::RegFile()
{
    for (uint8_t i = 0; i < NUM_REGISTERS; i++)
        this->registers[i] = 0;
}

uint32_t RegFile::readRegister(uint8_t registerNum) const
{
    return this->registers[registerNum];
}

void RegFile::writeRegister(uint8_t registerNum, uint32_t data)
{
    // X0 is hardwired to 0.
    if (registerNum == 0)
        return;
    this->registers[registerNum] = data;
}
