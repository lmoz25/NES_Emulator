#include "CPU.h"
#include "CPU_Timer.h"
#include "ThreadUtils.h"

namespace cpu {

CPU::CPU() : X(0), Y(0), accumulator(0), processor_status(0), stack_pointer(STACK_START), program_counter(0){}

void CPU::loadROM(std::string& path){
    std::ifstream gamefile;
    gamefile.open(path.c_str());    
    uint8_t ch = gamefile.get();
    int i = ROM_START;
    // Rest of memory is rom data
    while (gamefile.good() && i < MEMORY_SIZE){
        memory_map.write(i, ch);
        ch = gamefile.get();
    }
    gamefile.close();
}

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

    switch (operation_tuple.operation)
    {
    case Operation::BRK:
        BRK();
        break;

    case Operation::ORA:
        ORA(operand);
        break;

    case Operation::ASL:
        ASL(operand);
        break;

    case Operation::PHP:
        PHP(operand);
        break;
    
    default:
        break;
    }
}

// ORA opcode
void CPU::ORA(std::bitset<8>& operand){
    accumulator |= operand;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

// ASL opcode
void CPU::ASL(std::bitset<8>& operand){
    // Set carry flag to value of bit 7
    processor_status.set(pFlag::CARRY, operand.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
    operand <<= 1;
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

void CPU::AND(std::bitset<8>& operand){
    accumulator &= operand;
    processor_status.set(pFlag::ZERO, accumulator == 0);
    // Store bit 7 in negative flag
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

void CPU::BIT(std::bitset<8>& operand){
    std::bitset<8> result = operand & accumulator;
    processor_status.set(pFlag::ZERO, result == 0);
    // Store bits 6 and 7 in overflow and negative flags resp.
    processor_status.set(pFlag::OVERFLOW, operand.test(6));
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

inline void CPU::ROL(std::bitset<8>& operand){
    // Store value of carry flag
    bool oldCarryFlag = processor_status.test(pFlag::CARRY);
    // Store bit 7 of the operand in the carry flag
    processor_status.set(pFlag::CARRY, operand.test(7));
    // Shift operand left
    operand <<= 1;
    // Set bit 0 of the operand to the old value of the carry flag
    operand.set(0, oldCarryFlag);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

inline void CPU::EOR(std::bitset<8>& operand){
    accumulator ^= operand;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

inline void CPU::LSR(std::bitset<8>& operand){
    processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>= 1;
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
    processor_status.set(pFlag::ZERO, operand == 0);
}

void CPU::ADC(std::bitset<8>& operand){
    // Check if addition results in a signed int outside of the bounds of an 8 bit signed int
    int16_t resultSignedInt = static_cast<int8_t>(operand.to_ulong()) +
                    static_cast<int8_t>(accumulator.to_ulong()) +
                    processor_status.test(pFlag::CARRY);

    processor_status.set(pFlag::OVERFLOW, 
                            resultSignedInt < INT8_MIN || resultSignedInt > INT8_MAX);

    // Now check if bit 7 has overflowed, i.e any of bits 8 or higher are set
    auto resultUnsignedInt = static_cast<uint16_t>(resultSignedInt);
    processor_status.set(pFlag::CARRY, resultUnsignedInt > UINT8_MAX);

    // Now set accumulator to this value, overflowed as an 8 bit unsigned int
    accumulator = resultUnsignedInt % UINT8_MAX;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

void CPU::ROR(std::bitset<8>& operand){
    bool oldCarryFlag = processor_status.test(pFlag::CARRY);
    processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>=1;
    // Set bit 7 of the operand to the old value of the carry flag
    operand.set(7, oldCarryFlag);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

void CPU::SBC(std::bitset<8>& operand){
    // Check if subtraction results in a signed int outside of the bounds of an 8 bit signed int
    int16_t resultSignedInt = static_cast<int8_t>(accumulator.to_ulong()) -
                              static_cast<int8_t>(operand.to_ulong()) -
                              !processor_status.test(pFlag::CARRY);

    processor_status.set(pFlag::OVERFLOW, 
                            resultSignedInt < INT8_MIN || resultSignedInt > INT8_MAX);

    // Now check if bit 7 has overflowed, i.e any of bits 8 or higher are set
    auto resultUnsignedInt = static_cast<uint16_t>(resultSignedInt);
    processor_status.set(pFlag::CARRY, resultUnsignedInt > UINT8_MAX);

    // Now set accumulator to this value, overflowed as an 8 bit unsigned int
    accumulator = resultUnsignedInt % UINT8_MAX;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

// void CPU::StoreRegister(std::bitset<8>& operand, Register8& reg, Register8&){
//     operand = reg;
// }

// void CPU::LoadRegister(std::bitset<8>& operand, Register8& reg, Register8& processor_status){
//     reg = operand;
//     processor_status.set(pFlag::ZERO, operand == 0);
//     processor_status.set(pFlag::NEGATIVE, operand.test(7));
// }

// void CPU::Decrement(std::bitset<8>& operand, Register8&, Register8& processor_status){
//     uint8_t operandUint = operand.to_ulong();
//     operandUint--;
//     operand = operandUint;
//     processor_status.set(pFlag::ZERO, operand == 0);
//     processor_status.set(pFlag::NEGATIVE, operand.test(7));
// }

// void CPU::Increment(std::bitset<8>& operand, Register8&, Register8& processor_status){
//     uint8_t operandUint = operand.to_ulong();
//     operandUint++;
//     operand = operandUint;
//     processor_status.set(pFlag::ZERO, operand == 0);
//     processor_status.set(pFlag::NEGATIVE, operand.test(7));
// }

// void CPU::CompareRegisterToMemory(std::bitset<8>& operand, Register8& reg, Register8& processor_status){
//     processor_status.set(pFlag::CARRY, reg.to_ulong() >= operand.to_ulong());
//     processor_status.set(pFlag::ZERO, reg == operand);
//     processor_status.set(pFlag::NEGATIVE, reg.to_ulong() < operand.to_ulong());
//}
} // cpu::