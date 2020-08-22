#include "Memory.h"

MemoryMap::MemoryMap() {
    memory.fill(0);
}

void MemoryMap::absoluteWrite(uint16_t program_counter, uint8_t value) {
    write(read(program_counter + 1) << 8 | read(program_counter), value);
}

void MemoryMap::immediateWrite(uint16_t program_counter, uint8_t value) {
    write(program_counter, value);
}

void MemoryMap::indexedWrite(uint16_t program_counter, uint8_t index, uint8_t value) {
    write(read(program_counter + 1) << 8 | (read(program_counter)  + index), value);
}

void MemoryMap::preIndexedIndirectWrite(uint16_t program_counter, uint8_t index, uint8_t value) {
    uint16_t address = preIndexGetAddress(program_counter, index);
    write(address, value);
}

void MemoryMap::postIndexedIndirectWrite(uint16_t program_counter, uint8_t index, uint8_t value) {
    uint16_t address = postIndexGetAddress(program_counter, index);
    write(address, value);
}

void MemoryMap::zeroPageWrite( uint16_t program_counter, uint8_t value) {
    write(value, program_counter);
}

void MemoryMap::zeroPageIndexedWrite(uint16_t program_counter, uint8_t value,  uint8_t index) {
    write(read(program_counter) + index, value);
}

uint8_t MemoryMap::absoluteRead(uint16_t program_counter) const{
    return read(read(program_counter + 1) << 8 | read(program_counter));
}

uint8_t MemoryMap::indexedRead(uint16_t program_counter, uint8_t index) const {
    return read(read(program_counter + 1) << 8 | (read(program_counter)  + index));
}

uint8_t MemoryMap::immediateRead(uint16_t program_counter) const{
    return read(program_counter);
}

uint8_t MemoryMap::zeroPageIndexedRead(uint16_t program_counter, uint8_t index) const{
    return read((read(program_counter) + index) % UINT8_MAX);
}

uint8_t MemoryMap::preIndexedIndirectRead(uint16_t program_counter, uint8_t index) const{
    uint16_t address = preIndexGetAddress(program_counter, index);
    return read(address);
}

uint8_t MemoryMap::postIndexedIndirectRead(uint16_t program_counter, uint8_t index) const{
    uint16_t address = postIndexGetAddress(program_counter, index);
    return read(address);
}

uint8_t MemoryMap::zeroPageRead(uint16_t program_counter) const{
    return read(read(program_counter));
}

uint16_t MemoryMap::absoluteReadPointer(uint16_t program_counter) const{
    return read(program_counter + 1) << 8 | read(program_counter);
}
uint16_t MemoryMap::indirectReadPointer(uint16_t program_counter) const{
    uint16_t address = 
        read(program_counter + 1) << 8 | read(program_counter);
    return read(address + 1 ) << 8 | read(address);
}