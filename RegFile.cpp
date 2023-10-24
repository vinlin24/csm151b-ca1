#include <iostream>

#include "RegFile.h"

using namespace std;

RegFile::RegFile()
{
    for (uint8_t i = 0; i < NUM_REGISTERS; i++)
        this->registers[i] = 0;
}

int32_t RegFile::readRegister(uint8_t registerNum) const
{
    return this->registers[registerNum];
}

void RegFile::writeRegister(uint8_t registerNum, int32_t data)
{
    // x0 is hardwired to 0.
    if (registerNum == 0)
    {
        cerr << "RegFile::writeRegister: attempt to write to x0 ignored"
             << endl;
        return;
    }
    this->registers[registerNum] = data;
    cerr << "RegFile::writeRegister: wrote " << data << " to x"
         << static_cast<int>(registerNum) << endl;
}
