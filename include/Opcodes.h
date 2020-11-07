#pragma once

#include <tuple>
#include <map>

namespace cpu {
enum class AddressingMode {
    ZERO_PAGE,
    PRE_INDEXED_INDIRECT,
    IMMEDIATE,
    ABSOLUTE,
    POST_INDEXED_INDIRECT,
    ZERO_PAGE_INDEXED_X,
    ZERO_PAGE_INDEXED_Y,
    INDEXED_X,
    INDEXED_Y,
    IMPLIED,
    ACCUMULATOR,
    INDIRECT
};

enum class Operation {
    BRK,
    ORA,
    ASL,
    PHP,
    BPL,
    CLC,
    JSL,
    AND,
    BIT,
    ROL,
    PLP,
    BMI,
    SEC,
    RTI,
    EOR,
    LSR,
    PHA,
    JMP,
    BVC,
    CLI,
    RTS,
    ADC,
    ROR,
    PLA,
    BVS,
    SET,
    STA,
    STY,
    STX,
    DEY,
    TXA,
    SDA,
    SDX,
    BCC,
    TYA,
    TXS,
    LDY,
    LDA,
    LDX,
    TAY,
    TAX,
    BCS,
    CLV,
    TSX,
    CPY,
    CMP,
    DEC,
    INY,
    DEX,
    BNE,
    CLD,
    CPX,
    SBC,
    INC,
    INX,
    NOP,
    BEQ,
    SED
};

struct OperationTuple {
    Operation operation;
    AddressingMode addressing_mode;
    uint8_t cycles;
    bool plus_if_crossed_page_boundary;
};

typedef uint8_t Opcode;
// Mapping between opcodes and everything the CPU needs to execute the opcodes
static std::map<Opcode, OperationTuple> 
 opcodes_to_operations = {
    {0x00, {Operation::BRK, AddressingMode::IMPLIED, 7, false}},
    {0x01, {Operation::ORA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x05, {Operation::ORA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x06, {Operation::ASL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x08, {Operation::PHP, AddressingMode::IMPLIED, 3, false}},
    {0x09, {Operation::ORA, AddressingMode::IMMEDIATE, 2, false}},
    {0x0a, {Operation::ASL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x0d, {Operation::ORA, AddressingMode::ABSOLUTE, 4, false}},
    {0x0e, {Operation::ASL, AddressingMode::ABSOLUTE, 6, false}},
    //TODO: +1 if branch taken and a further +1 if crossing page boundary
    {0x10, {Operation::BPL, AddressingMode::IMPLIED, 2, true}},
    {0x11, {Operation::ORA, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x15, {Operation::ORA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x16, {Operation::ASL, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x18, {Operation::CLC, AddressingMode::IMPLIED, 2, false}},
    {0x19, {Operation::ORA, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x1d, {Operation::ORA, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x1e, {Operation::ASL, AddressingMode::INDEXED_X, 7, false}},
    {0x20, {Operation::JSL, AddressingMode::ABSOLUTE, 6, false}},
    {0x21, {Operation::AND, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x24, {Operation::BIT, AddressingMode::ZERO_PAGE, 3, false}},
    {0x25, {Operation::AND, AddressingMode::ZERO_PAGE, 3, false}},
    {0x26, {Operation::ROL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x28, {Operation::PLP, AddressingMode::IMPLIED, 4, false}},
    {0x29, {Operation::AND, AddressingMode::IMMEDIATE, 2, false}},
    {0x2a, {Operation::ROL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x2c, {Operation::BIT, AddressingMode::ABSOLUTE, 4, false}},
    {0x2d, {Operation::AND, AddressingMode::ABSOLUTE, 4, false}},
    {0x2e, {Operation::ROL, AddressingMode::ABSOLUTE, 6, false}},
    {0x30, {Operation::BMI, AddressingMode::IMPLIED, 2, true}}, // TODO as above
    {0x31, {Operation::AND, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x35, {Operation::AND, AddressingMode::PRE_INDEXED_INDIRECT, 4, false}},
    {0x36, {Operation::ROL, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x38, {Operation::SEC, AddressingMode::IMPLIED, 2, false}},
    {0x39, {Operation::AND, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x3d, {Operation::AND, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x3e, {Operation::ROL, AddressingMode::INDEXED_X, 7, false}},
    {0x40, {Operation::RTI, AddressingMode::IMPLIED, 6, false}},
    {0x41, {Operation::EOR, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x45, {Operation::EOR, AddressingMode::ZERO_PAGE, 3, false}},
    {0x46, {Operation::LSR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x48, {Operation::PHA, AddressingMode::IMPLIED, 3, false}},
    {0x49, {Operation::EOR, AddressingMode::IMMEDIATE, 2, false}},
    {0x4a, {Operation::LSR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x4c, {Operation::JMP, AddressingMode::ABSOLUTE, 3, false}},
    {0x4d, {Operation::EOR, AddressingMode::ABSOLUTE, 4, false}},
    {0x4e, {Operation::LSR, AddressingMode::ABSOLUTE, 6, false}},
    {0x50, {Operation::BVC, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x51, {Operation::EOR, AddressingMode::POST_INDEXED_INDIRECT, 4, true}},
    {0x55, {Operation::EOR, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x56, {Operation::LSR, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x58, {Operation::CLI, AddressingMode::IMPLIED, 2, false}},
    {0x59, {Operation::EOR, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x5d, {Operation::EOR, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x5e, {Operation::LSR, AddressingMode::INDEXED_X, 7, false}},
    {0x60, {Operation::RTS, AddressingMode::IMPLIED, 6, false}},
    {0x61, {Operation::ADC, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x65, {Operation::ADC, AddressingMode::ZERO_PAGE, 3, false}},
    {0x66, {Operation::ROR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x68, {Operation::PLA, AddressingMode::IMPLIED, 4, false}},
    {0x69, {Operation::ADC, AddressingMode::IMMEDIATE, 2, false}},
    {0x6a, {Operation::ROR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x6c, {Operation::JMP, AddressingMode::INDIRECT, 5, false}},
    {0x6d, {Operation::ADC, AddressingMode::ABSOLUTE, 4, false}},
    {0x6e, {Operation::ROR, AddressingMode::ABSOLUTE, 6, false}},
    {0x70, {Operation::BVS, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x71, {Operation::ADC, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x75, {Operation::ADC, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x76, {Operation::ROR, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x78, {Operation::SET, AddressingMode::IMPLIED, 2, false}},
    {0x79, {Operation::ADC, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x7d, {Operation::ADC, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x7e, {Operation::ROR, AddressingMode::INDEXED_X, 7, false}},
    {0x81, {Operation::STA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x84, {Operation::STY, AddressingMode::ZERO_PAGE, 3, false}},
    {0x85, {Operation::STA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x86, {Operation::STX, AddressingMode::ZERO_PAGE, 3, false}},
    {0x88, {Operation::DEY, AddressingMode::IMPLIED, 2, false}},
    {0x8a, {Operation::TXA, AddressingMode::IMPLIED, 2, false}},
    {0x8c, {Operation::STY, AddressingMode::ABSOLUTE, 4, false}},
    {0x8d, {Operation::SDA, AddressingMode::ABSOLUTE, 4, false}},
    {0x8e, {Operation::SDX, AddressingMode::ABSOLUTE, 4, false}},
    {0x90, {Operation::BCC, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x91, {Operation::STA, AddressingMode::POST_INDEXED_INDIRECT, 6, false}},
    {0x94, {Operation::STY, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x95, {Operation::STA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x96, {Operation::STX, AddressingMode::ZERO_PAGE_INDEXED_Y, 4, false}},
    {0x98, {Operation::TYA, AddressingMode::IMPLIED, 2, false}},
    {0x99, {Operation::STA, AddressingMode::INDEXED_Y, 5, false}},
    {0x9a, {Operation::TXS, AddressingMode::IMPLIED, 2, false}},
    {0x9d, {Operation::STA, AddressingMode::INDEXED_X, 5, false}},
    {0xa0, {Operation::LDY, AddressingMode::IMMEDIATE, 2, false}},
    {0xa1, {Operation::LDA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xa2, {Operation::LDX, AddressingMode::IMMEDIATE, 2, false}},
    {0xa4, {Operation::LDY, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa5, {Operation::LDA, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa6, {Operation::LDX, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa8, {Operation::TAY, AddressingMode::IMPLIED, 2, false}},
    {0xa9, {Operation::LDA, AddressingMode::IMMEDIATE, 2, false}},
    {0xaa, {Operation::TAX, AddressingMode::IMPLIED, 2, false}},
    {0xac, {Operation::LDY, AddressingMode::ABSOLUTE, 4, false}},
    {0xad, {Operation::LDA, AddressingMode::ABSOLUTE, 4, false}},
    {0xae, {Operation::LDX, AddressingMode::ABSOLUTE, 4, false}},
    {0xb0, {Operation::BCS, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xb1, {Operation::LDA, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xb4, {Operation::LDY, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xb5, {Operation::LDA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xb6, {Operation::LDX, AddressingMode::ZERO_PAGE_INDEXED_Y, 4, false}},
    {0xb8, {Operation::CLV, AddressingMode::IMPLIED, 2, false}},
    {0xb9, {Operation::LDA, AddressingMode::INDEXED_Y, 4, true}},
    {0xba, {Operation::TSX, AddressingMode::IMPLIED, 2, false}},
    {0xbc, {Operation::LDY, AddressingMode::INDEXED_X, 4, true}},
    {0xbd, {Operation::LDA, AddressingMode::INDEXED_X, 4, true}},
    {0xbe, {Operation::LDX, AddressingMode::INDEXED_Y, 4, true}},
    {0xc0, {Operation::CPY, AddressingMode::IMMEDIATE, 2, false}},
    {0xc1, {Operation::CMP, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xc4, {Operation::CPY, AddressingMode::ZERO_PAGE, 3, false}},
    {0xc5, {Operation::CMP, AddressingMode::ZERO_PAGE, 3, false}},
    {0xc6, {Operation::DEC, AddressingMode::ZERO_PAGE, 5, false}},
    {0xc8, {Operation::INY, AddressingMode::IMPLIED, 2, false}},
    {0xc9, {Operation::CMP, AddressingMode::IMMEDIATE, 2, false}},
    {0xca, {Operation::DEX, AddressingMode::IMPLIED, 2, false}},
    {0xcc, {Operation::CPY, AddressingMode::ABSOLUTE, 4, false}},
    {0xcd, {Operation::CMP, AddressingMode::ABSOLUTE, 4, false}},
    {0xce, {Operation::DEC, AddressingMode::ABSOLUTE, 6, false}},
    {0xd0, {Operation::BNE, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xd1, {Operation::CMP, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xd5, {Operation::CMP, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xd6, {Operation::DEC, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0xd8, {Operation::CLD, AddressingMode::IMPLIED, 2, false}},
    {0xd9, {Operation::CMP, AddressingMode::INDEXED_Y, 4, true}},
    {0xdd, {Operation::CMP, AddressingMode::INDEXED_X, 4, true}},
    {0xde, {Operation::DEC, AddressingMode::INDEXED_X, 7, false}},
    {0xe0, {Operation::CPX, AddressingMode::IMMEDIATE, 2, false}},
    {0xe1, {Operation::SBC, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xe4, {Operation::CPX, AddressingMode::ZERO_PAGE, 3, false}},
    {0xe5, {Operation::SBC, AddressingMode::ZERO_PAGE, 3, false}},
    {0xe6, {Operation::INC, AddressingMode::ZERO_PAGE, 5, false}},
    {0xe8, {Operation::INX, AddressingMode::IMPLIED, 2, false}},
    {0xe9, {Operation::SBC, AddressingMode::IMMEDIATE, 2, false}},
    {0xea, {Operation::NOP, AddressingMode::IMMEDIATE, 2, false}},
    {0xec, {Operation::CPX, AddressingMode::ABSOLUTE, 4, false}},
    {0xed, {Operation::SBC, AddressingMode::ABSOLUTE, 4, false}},
    {0xee, {Operation::INC, AddressingMode::ABSOLUTE, 6, false}},
    {0xf0, {Operation::BEQ, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xf1, {Operation::SBC, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xf5, {Operation::SBC, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xf6, {Operation::INC, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0xf8, {Operation::SED, AddressingMode::IMPLIED, 2, false}},
    {0xf9, {Operation::SBC, AddressingMode::INDEXED_Y, 4, true}},
    {0xfd, {Operation::SBC, AddressingMode::INDEXED_X, 4, true}},
    {0xfe, {Operation::INC, AddressingMode::INDEXED_X, 7, false}},
};
} // ::cpu