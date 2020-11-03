#pragma once

#include "Operations.h"
#include "AddressingModes.h"
#include <tuple>

typedef uint8_t Opcode;
typedef uint8_t Cycles;
typedef bool PlusIfCrossPageBoundary;

// Mapping between opcodes and everything the CPU needs to execute the opcodes
std::map<Opcode, std::tuple<Operation, AddressingMode, Cycles, PlusIfCrossPageBoundary>> 
 opcodes = {
    {0x00, {operation::BRK, AddressingMode::IMPLIED, 7, false}},
    {0x01, {operation::ORA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x05, {operation::ORA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x06, {operation::ASL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x08, {operation::PHP, AddressingMode::IMPLIED, 3, false}},
    {0x09, {operation::ORA, AddressingMode::IMMEDIATE, 2, false}},
    {0x0a, {operation::ASL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x0d, {operation::ORA, AddressingMode::ABSOLUTE, 4, false}},
    {0x0e, {operation::ASL, AddressingMode::ABSOLUTE, 6, false}},
    //TODO: +1 if branch taken and a further +1 if crossing page boundary
    {0x10, {operation::BPL, AddressingMode::IMPLIED, 2, true}},
    {0x11, {operation::ORA, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x15, {operation::ORA, AddressingMode::ZERO_PAGE_INDEXED, 4, false}},
    {0x16, {operation::ASL, AddressingMode::ZERO_PAGE_INDEXED, 6, false}},
    {0x18, {operation::CLC, AddressingMode::IMPLIED, 2, false}},
    {0x19, {operation::ORA, AddressingMode::INDEXED, 4, true}}, // Indexed with Y
    {0x1d, {operation::ORA, AddressingMode::INDEXED, 4, true}}, // Indexed with X
    {0x1e, {operation::ASL, AddressingMode::INDEXED, 7, false}},
    {0x20, {operation::JSL, AddressingMode::ABSOLUTE, 6, false}},
    {0x21, {operation::AND, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x24, {operation::BIT, AddressingMode::ZERO_PAGE, 3, false}},
    {0x25, {operation::AND, AddressingMode::ZERO_PAGE, 3, false}},
    {0x26, {operation::ROL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x28, {operation::PLP, AddressingMode::IMPLIED, 4, false}},
    {0x29, {operation::AND, AddressingMode::IMMEDIATE, 2, false}},
    {0x2a, {operation::ROL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x2c, {operation::BIT, AddressingMode::ABSOLUTE, 4, false}},
    {0x2d, {operation::AND, AddressingMode::ABSOLUTE, 4, false}},
    {0x2e, {operation::ROL, AddressingMode::ABSOLUTE, 6, false}},
    {0x30, {operation::BMI, AddressingMode::IMPLIED, 2, true}}, // TODO as above
    {0x31, {operation::AND, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x35, {operation::AND, AddressingMode::PRE_INDEXED_INDIRECT, 4, false}},
    {0x36, {operation::ROL, AddressingMode::ZERO_PAGE_INDEXED, 6, false}},
    {0x38, {operation::SEC, AddressingMode::IMPLIED, 2, false}},
    {0x39, {operation::AND, AddressingMode::INDEXED, 4, true}}, // Indexed with Y
    {0x3d, {operation::AND, AddressingMode::INDEXED, 4, true}}, // Indexed with X
    {0x3e, {operation::ROL, AddressingMode::INDEXED, 7, false}},
    {0x40, {operation::RTI, AddressingMode::IMPLIED, 6, false}},
    {0x41, {operation::EOR, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x45, {operation::EOR, AddressingMode::ZERO_PAGE, 3, false}},
    {0x46, {operation::LSR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x48, {operation::PHA, AddressingMode::IMPLIED, 3, false}},
    {0x49, {operation::EOR, AddressingMode::IMMEDIATE, 2, false}},
    {0x4a, {operation::LSR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x4c, {operation::JMP, AddressingMode::ABSOLUTE, 3, false}},
    {0x4d, {operation::EOR, AddressingMode::ABSOLUTE, 4, false}},
    {0x4e, {operation::LSR, AddressingMode::ABSOLUTE, 6, false}},
    {0x50, {operation::BVC, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x51, {operation::EOR, AddressingMode::POST_INDEXED_INDIRECT, 4, true}},
    {0x55, {operation::EOR, AddressingMode::ZERO_PAGE_INDEXED, 4, false}},
    {0x56, {operation::LSR, AddressingMode::ZERO_PAGE_INDEXED, 6, false}},
    {0x58, {operation::CLI, AddressingMode::IMPLIED, 2, false}},
    {0x59, {operation::EOR, AddressingMode::INDEXED, 4, true}}, // Indexed with Y
    {0x5d, {operation::EOR, AddressingMode::INDEXED, 4, true}}, // Indexed with X
    {0x5e, {operation::LSR, AddressingMode::INDEXED, 7, false}},
    {0x60, {operation::RTS, AddressingMode::IMPLIED, 6, false}},
    {0x61, {operation::ADC, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x65, {operation::ADC, AddressingMode::ZERO_PAGE, 3, false}},
    {0x66, {operation::ROR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x68, {operation::PLA, AddressingMode::IMPLIED, 4, false}},
    {0x69, {operation::ADC, AddressingMode::IMMEDIATE, 2, false}},
    {0x6a, {operation::ROR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x6c, {operation::JMP, AddressingMode::INDIRECT, 5, false}},
    {0x6d, {operation::ADC, AddressingMode::ABSOLUTE, 4, false}},
    {0x6e, {operation::ROR, AddressingMode::ABSOLUTE, 6, false}},
    {0x70, {operation::BVS, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x71, {operation::ADC, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x75, {operation::ADC, AddressingMode::ZERO_PAGE_INDEXED, 4, false},
    {0x76, {operation::ROR, AddressingMode::ZERO_PAGE_INDEXED, 6, false}},
    {0x78, {operation::SET, AddressingMode::IMPLIED, 2, false}},
    {0x79, {operation::ADC, AddressingMode::INDEXED, 4, true}}, // Indexed with Y
    {0x7d, {operation::ADC, AddressingMode::INDEXED, 4, true}}, // Indexed with X
    {0x7e, {operation::ROR, AddressingMode::INDEXED, 7, false}},
    {0x81, {operation::STA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x84, {operation::STY, AddressingMode::ZERO_PAGE, 3, false}},
    {0x85, {operation::STA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x86, {operation::STX, AddressingMode::ZERO_PAGE, 3, false}},
    {0x88, {operation::DEY, AddressingMode::IMPLIED, 2, false}},
    {0x8a, {operation::TXA, AddressingMode::IMPLIED, 2, false}},
    {0x8c, {operation::STY, AddressingMode::ABSOLUTE, 4, false}},
    0x






}