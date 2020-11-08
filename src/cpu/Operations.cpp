#include "CPU.h"

// File to hold CPU operations, because CPU.cpp is too big as it is

namespace cpu {
/**
 * @brief Causes a non-maskable interrupt and increments the program counter by one
 * 
 * @param cpu_
 */
void CPU::BRK(CPU& cpu_, Operand&) {
    // Increment program counter and push to stack
    cpu_.pushToStack(cpu_.program_counter + 2);

    // Set interrupt flag, push status reg to stack
    cpu_.processor_status.set(pFlag::INTERRUPT);

    cpu_.pushToStack(static_cast<uint8_t>(cpu_.processor_status.to_ulong()));

    //Reload program counter
    cpu_.program_counter = 
        *cpu_.memory_map.immediateRead(0xFFFF) << 8 | *cpu_.memory_map.immediateRead(0xFFFE);
}

/**
 * @brief Bitwise OR with accumulator
 * 
 * @param cpu_
 * @param operand
 */
void CPU::ORA(CPU& cpu_, Operand& operand) {
    cpu_.accumulator |= operand;
    cpu_.processor_status.set(pFlag::NEGATIVE, cpu_.accumulator.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
}

/**
 * @brief Arithmetic shift left of operand
 * 
 * @param cpu_
 * @param operand
 */
void CPU::ASL(CPU& cpu_, Operand& operand) {
    // Set carry flag to value of bit 7
    cpu_.processor_status.set(pFlag::CARRY, operand.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
    operand <<= 1;
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

/**
 * @brief Push processor status to stack
 * 
 * @param cpu_
 * @param operand
 */
void CPU::PHP(CPU& cpu_, Operand&) {
    cpu_.pushToStack(static_cast<uint8_t>(cpu_.processor_status.to_ulong()));
    cpu_.program_counter++;
}

/**
 * @brief Logical AND with accumulator
 * 
 * @param cpu_
 * @param operand
 */
void CPU::AND(CPU& cpu_, Operand& operand) {
    cpu_.accumulator &= operand;
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
    // Store bit 7 in negative flag
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
}


/**
 * @brief Test if certain bits are present in the accumulator
 * 
 * @param cpu_
 * @param operand
 */
void CPU::BIT(CPU& cpu_, Operand& operand) {
    std::bitset<8> result = operand & cpu_.accumulator;
    cpu_.processor_status.set(pFlag::ZERO, result == 0);
    // Store bits 6 and 7 in overflow and negative flags resp.
    cpu_.processor_status.set(pFlag::OVERFLOW, operand.test(6));
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

/**
 * @brief Rotate bits left
 * 
 * @param cpu_
 * @param operand
 */
void CPU::ROL(CPU& cpu_, Operand& operand) {
    // Store value of carry flag
    bool old_carry_flag = cpu_.processor_status.test(pFlag::CARRY);
    // Store bit 7 of the operand in the carry flag
    cpu_.processor_status.set(pFlag::CARRY, operand.test(7));
    // Shift operand left
    operand <<= 1;
    // Set bit 0 of the operand to the old value of the carry flag
    operand.set(0, old_carry_flag);
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
}

/**
 * @brief Exclusive or with accumulator
 * 
 * @param cpu_
 * @param operand
 */
void CPU::EOR(CPU& cpu_, Operand& operand) {
    cpu_.accumulator ^= operand;
    cpu_.processor_status.set(pFlag::NEGATIVE, cpu_.accumulator.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
}

/**
 * @brief Logical shift right of the operand
 * 
 * @param cpu_
 * @param operand
 */
void CPU::LSR(CPU& cpu_, Operand& operand) {
    cpu_.processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>= 1;
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
    cpu_.processor_status.set(pFlag::ZERO, operand == 0);
}

/**
 * @brief Add to the accumulator, with carry
 * 
 * @param cpu_
 * @param operand
 */
void CPU::ADC(CPU& cpu_, Operand& operand) {
    // Check if addition results in a signed int outside of the bounds of an 8 bit signed int
    int16_t result_signed_int = static_cast<int8_t>(operand.to_ulong()) +
                    static_cast<int8_t>(cpu_.accumulator.to_ulong()) +
                    cpu_.processor_status.test(pFlag::CARRY);

    cpu_.processor_status.set(pFlag::OVERFLOW, 
                            result_signed_int < INT8_MIN || result_signed_int > INT8_MAX);

    // Now check if bit 7 has overflowed, i.e any of bits 8 or higher are set
    auto result_unsigned_int = static_cast<uint16_t>(result_signed_int);
    cpu_.processor_status.set(pFlag::CARRY, result_unsigned_int > UINT8_MAX);

    // Now set accumulator to this value, overflowed as an 8 bit unsigned int
    cpu_.accumulator = result_unsigned_int % UINT8_MAX;
    cpu_.processor_status.set(pFlag::NEGATIVE, cpu_.accumulator.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
}

void CPU::ROR(CPU& cpu_, Operand& operand) {
    bool old_carry_flag = cpu_.processor_status.test(pFlag::CARRY);
    cpu_.processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>=1;
    // Set bit 7 of the operand to the old value of the carry flag
    operand.set(7, old_carry_flag);
    cpu_.processor_status.set(pFlag::NEGATIVE, operand.test(7));
    cpu_.processor_status.set(pFlag::ZERO, cpu_.accumulator == 0);
}
}