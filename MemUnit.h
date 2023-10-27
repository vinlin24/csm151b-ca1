#ifndef MEMUNIT_H_INCLUDED
#define MEMUNIT_H_INCLUDED

#include <cstdint>

/**
 * Size of the data memory available to the CPU, measured in bytes.
 */
#define DATA_MEMORY_SIZE 4096

/**
 * Utility function for returning a 32-bit integer from 4 bytes loaded in Little
 * Endian fashion from a byte array.
 */
int32_t fromLittleEndian(uint8_t const *memoryAddress);

/**
 * Utility function for converting a 32-bit integer and writing it in Little
 * Endian fashion to a byte array.
 */
void toLittleEndian(int32_t data, uint8_t *memoryAddress);

class MemUnit
{
public:
    MemUnit();
    int32_t readData(uint32_t address) const;
    void writeData(uint32_t address, int32_t data);

private:
    /**
     * Data memory byte addressable in Little Endian format.
     */
    uint8_t dataMemory[DATA_MEMORY_SIZE];
};

#endif // MEMUNIT_H_INCLUDED
