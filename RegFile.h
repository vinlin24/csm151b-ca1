#ifndef REGFILE_H_INCLUDED
#define REGFILE_H_INCLUDED

#include <stdint.h>

#define NUM_REGISTERS 32

class RegFile
{
public:
    RegFile();
    int32_t readRegister(uint8_t registerNum) const;
    void writeRegister(uint8_t registerNum, int32_t data);

private:
    int32_t registers[NUM_REGISTERS];
};

#endif // REGFILE_H_INCLUDED
