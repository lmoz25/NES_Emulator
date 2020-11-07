#pragma once 

#include <array>
#include <bitset>
#include <functional>
#include <optional>

#include "Logger.h"
#include "Memory.h"
#include "Expections.h"
#include "Opcodes.h"

namespace cpu {

typedef std::bitset<8> Register8;

// Bit locations of flags in processor status register
enum pFlag {
    CARRY,
    ZERO,
    INTERRUPT,
    DECIMAL,
    BREAK,
    ALWAYS1,
    OVERFLOW,
    NEGATIVE
};

class CPU {
public:
    CPU();
    void loadROM(std::string& path);
    void processNextOpcode();

private:    
    void performOperation(const OperationTuple& operation);
    std::bitset<8> getOperandFromMemory(const AddressingMode& addressing_mode);
    inline void setProcessorStatus(pFlag flag, bool value){
        processor_status.set(flag, value);
    }

    inline uint8_t getProcessorStatus(pFlag pflag){
        return processor_status.test(static_cast<std::size_t>(pflag));
    }

    typedef std::bitset<8> Operand;
    // Operations
    void BRK();
    void ORA(Operand& operand);
    void ASL(Operand& operand);
    void PHP(Operand& operand);
    void AND(Operand& operand);
    void BIT(Operand& operand);
    void ROL(Operand& operand);
    void EOR(Operand& operand);
    void LSR(Operand& operand);
    void ADC(Operand& operand);
    void ROR(Operand& operand);
    void SBC(Operand& operand);

    MemoryMap memory_map;
    // 8-bit register
    Register8 X;
    Register8 Y;
    Register8 accumulator;
    // Made up of flags defined in pFlag, access with member functions
    Register8 processor_status;
    // 16-bit register
    uint16_t stack_pointer;
    uint16_t program_counter;

};
} // namespace cpu