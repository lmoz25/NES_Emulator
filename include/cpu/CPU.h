#pragma once 

#include <array>
#include <bitset>
#include <functional>
#include <optional>
#include <map>
#include <memory>

#include "Logger.h"
#include "Memory.h"
#include "Expections.h"

namespace cpu {
class CPU {
public:
    CPU();
    void processNextOpcode();

private:

    typedef std::bitset<8> Register8;
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


    typedef std::bitset<8> Operand;
    typedef std::function<void(CPU&, Operand&)> Operator;

    // Tuple holding information about CPU operations
    struct OperationTuple {
        Operator op;
        AddressingMode addressing_mode;
        uint8_t cycles;
        bool plus_if_crossed_page_boundary;
    };

    void performOperation(const OperationTuple& operation);
    std::shared_ptr<std::bitset<8>> getOperandFromMemory(const AddressingMode& addressing_mode);
    inline void setProcessorStatus(pFlag flag, bool value){
        processor_status.set(flag, value);
    }

    inline uint8_t getProcessorStatus(pFlag pflag){
        return processor_status.test(static_cast<std::size_t>(pflag));
    }

    inline void pushToStack(uint8_t value){
        memory_map.write(value, --stack_pointer);
    }

    inline uint8_t pullFromStack(){
        return *memory_map.read(stack_pointer++);
    }

    // Operations
    // TODO figure out a nice way to remove these from CPU class
    static void BRK(CPU& cpu_, Operand&);
    static void ORA(CPU& cpu_, Operand& operand);
    static void ASL(CPU& cpu_, Operand& operand);
    static void PHP(CPU& cpu_, Operand&);
    static void AND(CPU& cpu_, Operand& operand);
    static void BIT(CPU& cpu_, Operand& operand);
    static void ROL(CPU& cpu_, Operand& operand);
    static void EOR(CPU& cpu_, Operand& operand);
    static void LSR(CPU& cpu_, Operand& operand);
    static void ADC(CPU& cpu_, Operand& operand);
    static void ROR(CPU& cpu_, Operand& operand);
    static void BPL(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CLC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void JSL(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BMI(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SEC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void RTI(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void JMP(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BVC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CLI(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void RTS(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void PLA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BVS(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SET(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void STA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void STY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void STX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void DEY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TXA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SDA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SDX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BCC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TYA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TXS(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void LDY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void LDA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void LDX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TAY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BCS(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CLV(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TAX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void TSX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CPY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CMP(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void DEC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void INY(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void PLP(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void PHA(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void DEX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BNE(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CLD(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void CPX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SBC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void INC(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void INX(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void NOP(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void BEQ(CPU& /*cpu_*/, Operand& /*operand*/){}
    static void SED(CPU& /*cpu_*/, Operand& /*operand*/){}

    typedef uint8_t Opcode;

    const static std::map<Opcode, OperationTuple> opcodes_to_operations;
    memory::MemoryMap memory_map;
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