#pragma once

#include <bitset>
#include <functional>
/*

namespace cpu { 
typedef std::function<void(std::bitset<8>&, Register8&, Register8&)> Operation;

namespace operation {

// ORA opcode
static inline void ORA(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    accumulator |= operand;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

// ASL opcode
static inline void ASL(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    // Set carry flag to value of bit 7
    processor_status.set(pFlag::CARRY, operand.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
    operand <<= 1;
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void AND(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    accumulator &= operand;
    processor_status.set(pFlag::ZERO, accumulator == 0);
    // Store bit 7 in negative flag
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void BIT(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    std::bitset<8> result = operand & accumulator;
    processor_status.set(pFlag::ZERO, result == 0);
    // Store bits 6 and 7 in overflow and negative flags resp.
    processor_status.set(pFlag::OVERFLOW, operand.test(6));
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void ROL(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
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

static inline void EOR(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    accumulator ^= operand;
    processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

static inline void LSR(std::bitset<8>& operand, Register8&, Register8& processor_status){
    processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>= 1;
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
    processor_status.set(pFlag::ZERO, operand == 0);
}

static inline void ADC(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
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

static inline void ROR(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
    bool oldCarryFlag = processor_status.test(pFlag::CARRY);
    processor_status.set(pFlag::CARRY, operand.test(0));
    operand >>=1;
    // Set bit 7 of the operand to the old value of the carry flag
    operand.set(7, oldCarryFlag);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
    processor_status.set(pFlag::ZERO, accumulator == 0);
}

static inline void SBC(std::bitset<8>& operand, Register8& accumulator, Register8& processor_status){
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

static inline void StoreRegister(std::bitset<8>& operand, Register8& reg, Register8&){
    operand = reg;
}

static inline void LoadRegister(std::bitset<8>& operand, Register8& reg, Register8& processor_status){
    reg = operand;
    processor_status.set(pFlag::ZERO, operand == 0);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void Decrement(std::bitset<8>& operand, Register8&, Register8& processor_status){
    uint8_t operandUint = operand.to_ulong();
    operandUint--;
    operand = operandUint;
    processor_status.set(pFlag::ZERO, operand == 0);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void Increment(std::bitset<8>& operand, Register8&, Register8& processor_status){
    uint8_t operandUint = operand.to_ulong();
    operandUint++;
    operand = operandUint;
    processor_status.set(pFlag::ZERO, operand == 0);
    processor_status.set(pFlag::NEGATIVE, operand.test(7));
}

static inline void CompareRegisterToMemory(std::bitset<8>& operand, Register8& reg, Register8& processor_status){
    processor_status.set(pFlag::CARRY, reg.to_ulong() >= operand.to_ulong());
    processor_status.set(pFlag::ZERO, reg == operand);
    processor_status.set(pFlag::NEGATIVE, reg.to_ulong() < operand.to_ulong());
}
} // operation::
} // cpu::
*/