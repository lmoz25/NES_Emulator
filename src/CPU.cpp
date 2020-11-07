#include "CPU.h"
#include "CPU_Timer.h"
#include "ThreadUtils.h"

namespace cpu {
// Mapping between opcodes and operation tuples
const std::map<CPU::Opcode, CPU::OperationTuple> CPU::opcodes_to_operations = {
    {0x00, {CPU::BRK, AddressingMode::IMPLIED, 7, false}},
    {0x01, {CPU::ORA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x05, {CPU::ORA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x06, {CPU::ASL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x08, {CPU::PHP, AddressingMode::IMPLIED, 3, false}},
    {0x09, {CPU::ORA, AddressingMode::IMMEDIATE, 2, false}},
    {0x0a, {CPU::ASL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x0d, {CPU::ORA, AddressingMode::ABSOLUTE, 4, false}},
    {0x0e, {CPU::ASL, AddressingMode::ABSOLUTE, 6, false}},
    //TODO: +1 if branch taken and a further +1 if crossing page boundary
    {0x10, {CPU::BPL, AddressingMode::IMPLIED, 2, true}},
    {0x11, {CPU::ORA, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x15, {CPU::ORA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x16, {CPU::ASL, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x18, {CPU::CLC, AddressingMode::IMPLIED, 2, false}},
    {0x19, {CPU::ORA, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x1d, {CPU::ORA, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x1e, {CPU::ASL, AddressingMode::INDEXED_X, 7, false}},
    {0x20, {CPU::JSL, AddressingMode::ABSOLUTE, 6, false}},
    {0x21, {CPU::AND, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x24, {CPU::BIT, AddressingMode::ZERO_PAGE, 3, false}},
    {0x25, {CPU::AND, AddressingMode::ZERO_PAGE, 3, false}},
    {0x26, {CPU::ROL, AddressingMode::ZERO_PAGE, 5, false}},
    {0x28, {CPU::PLP, AddressingMode::IMPLIED, 4, false}},
    {0x29, {CPU::AND, AddressingMode::IMMEDIATE, 2, false}},
    {0x2a, {CPU::ROL, AddressingMode::ACCUMULATOR, 2, false}},
    {0x2c, {CPU::BIT, AddressingMode::ABSOLUTE, 4, false}},
    {0x2d, {CPU::AND, AddressingMode::ABSOLUTE, 4, false}},
    {0x2e, {CPU::ROL, AddressingMode::ABSOLUTE, 6, false}},
    {0x30, {CPU::BMI, AddressingMode::IMPLIED, 2, true}}, // TODO as above
    {0x31, {CPU::AND, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x35, {CPU::AND, AddressingMode::PRE_INDEXED_INDIRECT, 4, false}},
    {0x36, {CPU::ROL, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x38, {CPU::SEC, AddressingMode::IMPLIED, 2, false}},
    {0x39, {CPU::AND, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x3d, {CPU::AND, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x3e, {CPU::ROL, AddressingMode::INDEXED_X, 7, false}},
    {0x40, {CPU::RTI, AddressingMode::IMPLIED, 6, false}},
    {0x41, {CPU::EOR, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x45, {CPU::EOR, AddressingMode::ZERO_PAGE, 3, false}},
    {0x46, {CPU::LSR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x48, {CPU::PHA, AddressingMode::IMPLIED, 3, false}},
    {0x49, {CPU::EOR, AddressingMode::IMMEDIATE, 2, false}},
    {0x4a, {CPU::LSR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x4c, {CPU::JMP, AddressingMode::ABSOLUTE, 3, false}},
    {0x4d, {CPU::EOR, AddressingMode::ABSOLUTE, 4, false}},
    {0x4e, {CPU::LSR, AddressingMode::ABSOLUTE, 6, false}},
    {0x50, {CPU::BVC, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x51, {CPU::EOR, AddressingMode::POST_INDEXED_INDIRECT, 4, true}},
    {0x55, {CPU::EOR, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x56, {CPU::LSR, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x58, {CPU::CLI, AddressingMode::IMPLIED, 2, false}},
    {0x59, {CPU::EOR, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x5d, {CPU::EOR, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x5e, {CPU::LSR, AddressingMode::INDEXED_X, 7, false}},
    {0x60, {CPU::RTS, AddressingMode::IMPLIED, 6, false}},
    {0x61, {CPU::ADC, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x65, {CPU::ADC, AddressingMode::ZERO_PAGE, 3, false}},
    {0x66, {CPU::ROR, AddressingMode::ZERO_PAGE, 5, false}},
    {0x68, {CPU::PLA, AddressingMode::IMPLIED, 4, false}},
    {0x69, {CPU::ADC, AddressingMode::IMMEDIATE, 2, false}},
    {0x6a, {CPU::ROR, AddressingMode::ACCUMULATOR, 2, false}},
    {0x6c, {CPU::JMP, AddressingMode::INDIRECT, 5, false}},
    {0x6d, {CPU::ADC, AddressingMode::ABSOLUTE, 4, false}},
    {0x6e, {CPU::ROR, AddressingMode::ABSOLUTE, 6, false}},
    {0x70, {CPU::BVS, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x71, {CPU::ADC, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0x75, {CPU::ADC, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x76, {CPU::ROR, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0x78, {CPU::SET, AddressingMode::IMPLIED, 2, false}},
    {0x79, {CPU::ADC, AddressingMode::INDEXED_Y, 4, true}}, // Indexed with Y
    {0x7d, {CPU::ADC, AddressingMode::INDEXED_X, 4, true}}, // Indexed with X
    {0x7e, {CPU::ROR, AddressingMode::INDEXED_X, 7, false}},
    {0x81, {CPU::STA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0x84, {CPU::STY, AddressingMode::ZERO_PAGE, 3, false}},
    {0x85, {CPU::STA, AddressingMode::ZERO_PAGE, 3, false}},
    {0x86, {CPU::STX, AddressingMode::ZERO_PAGE, 3, false}},
    {0x88, {CPU::DEY, AddressingMode::IMPLIED, 2, false}},
    {0x8a, {CPU::TXA, AddressingMode::IMPLIED, 2, false}},
    {0x8c, {CPU::STY, AddressingMode::ABSOLUTE, 4, false}},
    {0x8d, {CPU::SDA, AddressingMode::ABSOLUTE, 4, false}},
    {0x8e, {CPU::SDX, AddressingMode::ABSOLUTE, 4, false}},
    {0x90, {CPU::BCC, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0x91, {CPU::STA, AddressingMode::POST_INDEXED_INDIRECT, 6, false}},
    {0x94, {CPU::STY, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x95, {CPU::STA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0x96, {CPU::STX, AddressingMode::ZERO_PAGE_INDEXED_Y, 4, false}},
    {0x98, {CPU::TYA, AddressingMode::IMPLIED, 2, false}},
    {0x99, {CPU::STA, AddressingMode::INDEXED_Y, 5, false}},
    {0x9a, {CPU::TXS, AddressingMode::IMPLIED, 2, false}},
    {0x9d, {CPU::STA, AddressingMode::INDEXED_X, 5, false}},
    {0xa0, {CPU::LDY, AddressingMode::IMMEDIATE, 2, false}},
    {0xa1, {CPU::LDA, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xa2, {CPU::LDX, AddressingMode::IMMEDIATE, 2, false}},
    {0xa4, {CPU::LDY, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa5, {CPU::LDA, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa6, {CPU::LDX, AddressingMode::ZERO_PAGE, 3, false}},
    {0xa8, {CPU::TAY, AddressingMode::IMPLIED, 2, false}},
    {0xa9, {CPU::LDA, AddressingMode::IMMEDIATE, 2, false}},
    {0xaa, {CPU::TAX, AddressingMode::IMPLIED, 2, false}},
    {0xac, {CPU::LDY, AddressingMode::ABSOLUTE, 4, false}},
    {0xad, {CPU::LDA, AddressingMode::ABSOLUTE, 4, false}},
    {0xae, {CPU::LDX, AddressingMode::ABSOLUTE, 4, false}},
    {0xb0, {CPU::BCS, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xb1, {CPU::LDA, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xb4, {CPU::LDY, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xb5, {CPU::LDA, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xb6, {CPU::LDX, AddressingMode::ZERO_PAGE_INDEXED_Y, 4, false}},
    {0xb8, {CPU::CLV, AddressingMode::IMPLIED, 2, false}},
    {0xb9, {CPU::LDA, AddressingMode::INDEXED_Y, 4, true}},
    {0xba, {CPU::TSX, AddressingMode::IMPLIED, 2, false}},
    {0xbc, {CPU::LDY, AddressingMode::INDEXED_X, 4, true}},
    {0xbd, {CPU::LDA, AddressingMode::INDEXED_X, 4, true}},
    {0xbe, {CPU::LDX, AddressingMode::INDEXED_Y, 4, true}},
    {0xc0, {CPU::CPY, AddressingMode::IMMEDIATE, 2, false}},
    {0xc1, {CPU::CMP, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xc4, {CPU::CPY, AddressingMode::ZERO_PAGE, 3, false}},
    {0xc5, {CPU::CMP, AddressingMode::ZERO_PAGE, 3, false}},
    {0xc6, {CPU::DEC, AddressingMode::ZERO_PAGE, 5, false}},
    {0xc8, {CPU::INY, AddressingMode::IMPLIED, 2, false}},
    {0xc9, {CPU::CMP, AddressingMode::IMMEDIATE, 2, false}},
    {0xca, {CPU::DEX, AddressingMode::IMPLIED, 2, false}},
    {0xcc, {CPU::CPY, AddressingMode::ABSOLUTE, 4, false}},
    {0xcd, {CPU::CMP, AddressingMode::ABSOLUTE, 4, false}},
    {0xce, {CPU::DEC, AddressingMode::ABSOLUTE, 6, false}},
    {0xd0, {CPU::BNE, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xd1, {CPU::CMP, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xd5, {CPU::CMP, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xd6, {CPU::DEC, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0xd8, {CPU::CLD, AddressingMode::IMPLIED, 2, false}},
    {0xd9, {CPU::CMP, AddressingMode::INDEXED_Y, 4, true}},
    {0xdd, {CPU::CMP, AddressingMode::INDEXED_X, 4, true}},
    {0xde, {CPU::DEC, AddressingMode::INDEXED_X, 7, false}},
    {0xe0, {CPU::CPX, AddressingMode::IMMEDIATE, 2, false}},
    {0xe1, {CPU::SBC, AddressingMode::PRE_INDEXED_INDIRECT, 6, false}},
    {0xe4, {CPU::CPX, AddressingMode::ZERO_PAGE, 3, false}},
    {0xe5, {CPU::SBC, AddressingMode::ZERO_PAGE, 3, false}},
    {0xe6, {CPU::INC, AddressingMode::ZERO_PAGE, 5, false}},
    {0xe8, {CPU::INX, AddressingMode::IMPLIED, 2, false}},
    {0xe9, {CPU::SBC, AddressingMode::IMMEDIATE, 2, false}},
    {0xea, {CPU::NOP, AddressingMode::IMMEDIATE, 2, false}},
    {0xec, {CPU::CPX, AddressingMode::ABSOLUTE, 4, false}},
    {0xed, {CPU::SBC, AddressingMode::ABSOLUTE, 4, false}},
    {0xee, {CPU::INC, AddressingMode::ABSOLUTE, 6, false}},
    {0xf0, {CPU::BEQ, AddressingMode::IMPLIED, 2, true}}, //TODO as above
    {0xf1, {CPU::SBC, AddressingMode::POST_INDEXED_INDIRECT, 5, true}},
    {0xf5, {CPU::SBC, AddressingMode::ZERO_PAGE_INDEXED_X, 4, false}},
    {0xf6, {CPU::INC, AddressingMode::ZERO_PAGE_INDEXED_X, 6, false}},
    {0xf8, {CPU::SED, AddressingMode::IMPLIED, 2, false}},
    {0xf9, {CPU::SBC, AddressingMode::INDEXED_Y, 4, true}},
    {0xfd, {CPU::SBC, AddressingMode::INDEXED_X, 4, true}},
    {0xfe, {CPU::INC, AddressingMode::INDEXED_X, 7, false}},
};

CPU::CPU()
    : X(0), Y(0), accumulator(0), processor_status(0), stack_pointer(STACK_START), program_counter(0){}

void CPU::processNextOpcode(){
    uint8_t opcode = memory_map.read(program_counter);

    auto op = opcodes_to_operations.find(opcode)->second;

    // Start the timer
    CPU_Timer timer(op.cycles);

    performOperation(op);

    if(op.plus_if_crossed_page_boundary /*&&
        memory_map.pageBoundaryCrossed()*/) {
        CPU_Timer::extra_cycles++;
    }

    std::unique_lock<std::mutex> lock(mtx);
    // Wait for timer to wake this thread - stop waiting after 10 6502 cycles:
    // no instruction should take that long.
    // TODO - maybe this shouldn't live here?
    // thread_waker.wait_until(lock, 
    //     std::chrono::system_clock::now() + 10*CPU_Timer::cpu_cycle_length_useconds);
}

void CPU::performOperation(const OperationTuple& operation_tuple) {
    auto operand = getOperandFromMemory(operation_tuple.addressing_mode);
   operation_tuple.op(*this, operand);
}


} // cpu::