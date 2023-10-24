#ifndef MEMUNIT_H_INCLUDED
#define MEMUNIT_H_INCLUDED

#include <stdint.h>

/**
 * Size of the data memory available to the CPU, measured in words (4-byte
 * blocks).
 */
#define DATA_MEMORY_SIZE 4096

class MemUnit
{
public:
    MemUnit();
    int32_t readData(uint32_t address) const;
    void writeData(uint32_t address, int32_t data);

private:
    int32_t dataMemory[DATA_MEMORY_SIZE];
};

#endif // MEMUNIT_H_INCLUDED
