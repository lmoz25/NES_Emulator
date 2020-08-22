#pragma once 

#include <array>
#include <bitset>
#include <functional>
#include <optional>

#include "Logger.h"
#include "Memory.h"
#include "Operations.h"
#include "Expections.h"

namespace cpu{

class CPU {
public:
    CPU();
    void loadROM(std::string& path);
    void processNextOpcode();

private:
    inline void setProcessorStatus(pFlag flag, bool value){
        processor_status.set(flag, value);
    }

    inline uint8_t getProcessorStatus(pFlag pFlag){
        return processor_status.test(pFlag);
    }

    /**
     * Readonly operations - perform the operation specified by op, with the value
     * taken from the given memory access method
     */

    inline void zeroPageOperation(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand = std::bitset<8>(memoryMap.zeroPageRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter++;
    }

    inline void preIndexedIndirectOperation(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.preIndexedIndirectRead(++program_counter, X.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter+=2;
    }

    inline void immediateOperation(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.immediateRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter++;
    }

    inline void absoluteOperation(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.absoluteRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter+=2;
    }

    inline void postIndexedIndirectOperation(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.postIndexedIndirectRead(++program_counter, Y.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter+=2;
    }
    
    inline void zeroPageIndexedOperation(Operator op, Register8 index, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.indexedRead(++program_counter, index.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter++;
    }

    inline void indexedOperation(Operator op, Register8 index, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.indexedRead(++program_counter, index.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        program_counter+=2;
    }

    /** Readwrite operations - perform the operation specified by op, with the value
     * taken from the given memory access method, then write the result back to that
     * memory location
     */
    inline void zeroPageOperationWithWrite(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand = std::bitset<8>(memoryMap.zeroPageRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.zeroPageWrite(program_counter, operand.to_ulong());
        program_counter++;
    }

    inline void preIndexedIndirectOperationWithWrite(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.preIndexedIndirectRead(++program_counter,
                                                                X.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.preIndexedIndirectWrite(program_counter,
                                            X.to_ulong(),
                                            operand.to_ulong());
        program_counter+=2;
    }

    inline void immediateOperationWithWrite(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.immediateRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.immediateWrite(program_counter, operand.to_ulong());
        program_counter++;
    }

    inline void absoluteOperationWithWrite(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.absoluteRead(++program_counter));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.immediateWrite(program_counter, operand.to_ulong());
        program_counter+=2;
    }

    inline void postIndexedIndirectOperationWithWrite(Operator op, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.postIndexedIndirectRead(++program_counter,
                                                                Y.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.postIndexedIndirectWrite(program_counter,
                                            Y.to_ulong(), 
                                            operand.to_ulong());
        program_counter+=2;
    }
    
    inline void zeroPageIndexedOperationWithWrite(Operator op, Register8 index, std::optional<Register8> reg = std::nullopt){
        auto operand =
            std::bitset<8>(memoryMap.zeroPageIndexedRead(++program_counter,
                                                            index.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.zeroPageIndexedWrite(program_counter,
                                        index.to_ulong(),
                                        operand.to_ulong());
        program_counter++;
    }

    inline void indexedOperationWithWrite(Operator op, Register8 index, std::optional<Register8> reg = std::nullopt){
        auto operand = std::bitset<8>(memoryMap.indexedRead(++program_counter, index.to_ulong()));
        op(operand, reg ? reg.value() : accumulator, processor_status);
        memoryMap.indexedWrite(program_counter, index.to_ulong(), operand.to_ulong());
        program_counter+=2;
    }

    // Accumulator operation, read/write depending on operator.
    inline void accumulatorOperation(Operator op){
        op(accumulator, accumulator, processor_status);
        program_counter++;
    }

    inline void pushToStack(uint8_t value){
        memoryMap.write(value, --stack_pointer);
    }

    inline uint8_t pullFromStack(){
        return memoryMap.read(stack_pointer++);
    }

    // Branch if the given flag equals the given condition
    inline void branchOnCondition(pFlag flag, uint8_t condition){
        if(getProcessorStatus(flag) == condition){
            program_counter += memoryMap.read(program_counter + 1);
        }
    }

    inline void transferRegisterToAccumulator(Register8 reg){
        accumulator = reg;
        processor_status.set(pFlag::ZERO, accumulator == 0);
        processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
    }

    inline void transferAccumulatorToRegister(Register8 reg){
        reg = accumulator;
        processor_status.set(pFlag::ZERO, reg == 0);
        processor_status.set(pFlag::NEGATIVE, reg.test(7));
    }

    inline void transferStackPointerToRegister(Register8 reg){
        reg = stack_pointer;
        processor_status.set(pFlag::ZERO, reg == 0);
        processor_status.set(pFlag::NEGATIVE, reg.test(7))
    }

    MemoryMap memoryMap;
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