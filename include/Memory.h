#pragma once
#include <array>

#include "Logger.h"
#define MEMORY_SIZE 0x10000
// descending stack so start is further along than end
#define STACK_START 0x1FF
#define STACK_END 0x100
#define ROM_START 0x8000 // takes up the rest of memory from here

namespace memory {
// Singleton class, allowing operations on RAM
struct MemoryMap{
    MemoryMap();

    /**
    * Convenience functions for read/write memory operations in different addressing modes.
    * Functions take in a program_counter, which corresponds to the program counter register
    * kept by the CPU. The actual addresses looked up using this program counter depends on
    * the addressing mode.
    */

    void absoluteWrite(uint16_t program_counter, uint8_t value);
    void immediateWrite(uint16_t program_counter, uint8_t value);
    void indexedWrite(uint16_t program_counter, uint8_t index, uint8_t value);
    void zeroPageWrite( uint16_t program_counter, uint8_t value);
    void zeroPageIndexedWrite(uint16_t program_counter, uint8_t index, uint8_t value);
    void postIndexedIndirectWrite(uint16_t program_counter, uint8_t index, uint8_t value);
    void preIndexedIndirectWrite(uint16_t program_counter, uint8_t index, uint8_t value);

    uint8_t* absoluteRead(uint16_t program_counter) const;
    uint8_t* immediateRead(uint16_t program_counter) const;
    uint8_t* indexedRead(uint16_t program_counter, uint8_t index) const;
    uint8_t* zeroPageRead(uint16_t program_counter) const;
    uint8_t* zeroPageIndexedRead(uint16_t program_counter, uint8_t index) const;
    uint8_t* postIndexedIndirectRead(uint16_t program_counter, uint8_t index) const;
    uint8_t* preIndexedIndirectRead(uint16_t program_counter, uint8_t index) const;

    /** 
     * Functions for returning an address during a memory operation, where
     * other functions return value at that pointer
     */

    uint16_t absoluteReadPointer(uint16_t program_counter) const;
    uint16_t indirectReadPointer(uint16_t program_counter) const;

    /**
     * Direct memory read/write
     */

    void write(uint16_t address, uint8_t value);
    uint8_t* read(uint16_t address) const;

private:
    uint16_t preIndexGetAddress(uint16_t program_counter, uint8_t index) const;

    uint16_t postIndexGetAddress(uint16_t program_counter, uint8_t index) const;

    static std::array<uint8_t, MEMORY_SIZE> memory_map;
};
} // memory::