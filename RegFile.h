#ifndef REGFILE_H_INCLUDED
#define REGFILE_H_INCLUDED

#include <stdint.h>

#define NUM_REGISTERS 32

class RegFile
{
public:
    RegFile();
    uint32_t readRegister(uint8_t registerNum) const;
    void writeRegister(uint8_t registerNum, uint32_t data);

private:
    uint32_t registers[NUM_REGISTERS];
};

#endif // REGFILE_H_INCLUDED
