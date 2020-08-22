#include "CPU.h"

namespace cpu {

CPU::CPU() : X(0), Y(0), accumulator(0), processor_status(0), stack_pointer(STACK_START), program_counter(0){
}

void CPU::loadROM(std::string& path){
    std::ifstream gamefile;
    gamefile.open(path.c_str());    
    uint8_t ch = gamefile.get();
    int i = ROM_START;
    // Rest of memory is rom data
    while (gamefile.good() && i < MEMORY_SIZE){
        memoryMap.write(i, ch);
        ch = gamefile.get();
    }
    gamefile.close();
}
void CPU::processNextOpcode(){
    uint8_t opcode = memoryMap.read(program_counter);
    switch (opcode){
       /**
        * BRK opcodes
        */
        case 0x00:
            // Increment program counter and push to stack
            pushToStack(program_counter + 2);
            // Set interrupt flag, push status reg to stack
            processor_status.set(pFlag::INTERRUPT);
            pushToStack(static_cast<uint8_t>(processor_status.to_ulong()));
            //Reload program counter
            program_counter = memoryMap.immediateRead(0xFFFE) << 8 | memoryMap.immediateRead(0xFFFF);
            break;

        /**
         * ORA opcodes - logical OR of operand with accumlator
         */
        case 0x01:
            preIndexedIndirectOperation(operation::ORA);
            break;
        
        case 0x05:
            zeroPageOperation(operation::ORA);
            break;
        
        case 0x09:
            immediateOperation(operation::ORA);
            break;
        
        case 0x0D:
            absoluteOperation(operation::ORA);
            break;
        
        case 0x11:
            postIndexedIndirectOperation(operation::ORA);
            break;
        
        case 0x15:
            zeroPageIndexedOperation(operation::ORA, X);
            break;
        
        case 0x19:
            zeroPageIndexedOperation(operation::ORA, Y);
            break;
        
        case 0x1D:
            indexedOperation(operation::ORA, X);
            break;
        

        /**
         * ASL opcodes - arithmetic shift left
         */
        case 0x06:
            zeroPageOperationWithWrite(operation::ASL);
            break;
        
        case 0x0A:
            accumulatorOperation(operation::ASL);
            break;
        
        case 0x0E:
            absoluteOperationWithWrite(operation::ASL);
            break;
        
        case 0x16:
            zeroPageIndexedOperationWithWrite(operation::ASL, X);
            break;
        
        case 0x1E:
            indexedOperationWithWrite(operation::ASL, X);
            break;
        

        /**
         * PHP opcode - push processor status
         */
        case 0x08:
            pushToStack(static_cast<uint8_t>(processor_status.to_ulong()));
            program_counter++;
            break;

        /**
         * BPL opcode - branch if positive
         */
        case 0x10:
            branchOnCondition(pFlag::NEGATIVE, 0);
            break;

        /**
         * CLC opcode - clear carry flag
         */
        case 0x18:
            processor_status.set(pFlag::CARRY, 0);
            break;

        /**
         * JSR opcode - jump to subroutine
         */
        case 0x20:
            pushToStack(program_counter - 1);
            program_counter = memoryMap.absoluteRead(program_counter + 1);
            break;

        /**
         * AND opcodes - logical and with accumulator
         */
        case 0x21:
            preIndexedIndirectOperation(operation::AND);
            break;
        
        case 0x25:
            zeroPageOperation(operation::AND);
            break;
        
        case 0x29:
            immediateOperation(operation::AND);
            break;
        
        case 0x2D:
            absoluteOperation(operation::AND);
        
        case 0x31:
            postIndexedIndirectOperation(operation::AND);
            break;
        
        case 0x35:
            zeroPageIndexedOperation(operation::AND, X);
            break;
        
        case 0x39:
            indexedOperation(operation::AND, Y);
            break;
        
        case 0x3D:
            indexedOperation(operation::AND, X);
            break;
        

        /**
         * BIT opcodes - test if certain bits are set in memory
         */
        case 0x24:
            zeroPageOperation(operation::BIT);
            break;
        
        case 0x2C:
            absoluteOperation(operation::BIT);
            break;
        

        /**
         * ROL opcodes - rotate operand or accumulator one bit left 
         */
        case 0x26:
            zeroPageOperationWithWrite(operation::ROL);
            break;
        
        case 0x2A:
            accumulatorOperation(operation::ROL);
            break;
        
        case 0x2E:
            absoluteOperationWithWrite(operation::ROL);
            break;
        
        case 0x36:
            zeroPageIndexedOperationWithWrite(operation::ROL, X);
            break;
        
        case 0x3E:
            indexedOperationWithWrite(operation::ROL, X);
            break;
        

        /**
         * PLP opcode - pull processor status from stack
         */
        case 0x28:
            processor_status = pullFromStack();
            program_counter++;
            break;

        /**
         * BMI opcode - branch if minus
         */
        case 0x30:
            branchOnCondition(pFlag::NEGATIVE, 1);
            break;
        
        /**
         * SEC opcode - set carry flag
         */
        case 0x38:
            processor_status.set(pFlag::CARRY);
            break;

        /**
         * RTI opcode - return from interrupt
         */
        case 0x40:
            processor_status = pullFromStack();
            program_counter = pullFromStack();
            break;

        /**
         * EOR opcode - exclusive or with the accumulator
         */
        case 0x41:
            preIndexedIndirectOperation(operation::EOR);
            break;

        case 0x45:
            zeroPageOperation(operation::EOR);
            break;

        case 0x49:
            immediateOperation(operation::EOR);
            break;

        case 0x4D:
            absoluteOperation(operation::EOR);
            break;

        case 0x51:
            postIndexedIndirectOperation(operation::EOR);
            break;

        case 0x55:
            zeroPageIndexedOperation(operation::EOR, X);
            break;

        case 0x59:
            indexedOperation(operation::EOR, Y);
            break;

        case 0x5D:
            indexedOperation(operation::EOR, X);
            break;

        /**
         * LSR opcodes - logical shift right of operand or accumulator
         */
        case 0x46:
            zeroPageOperationWithWrite(operation::LSR);
            break;

        case 0x4A:
            accumulatorOperation(operation::LSR);
            break;

        case 0x4E:
            absoluteOperation(operation::LSR);
            break;

        case 0x56:
            zeroPageIndexedOperationWithWrite(operation::LSR, X);
            break;

        case 0x5E:
            indexedOperationWithWrite(operation::LSR, X);
            break;

        /**
         * PHA opcode - push accumulator to stack
         */
        case 0x48:
            pushToStack(accumulator.to_ulong());
            break;

        /**
         * JMP opcode - Jump to address
         */
        case 0x4C:
            program_counter = memoryMap.absoluteReadPointer(program_counter);
            break;

        case 0x5C:
            program_counter = memoryMap.indirectReadPointer(program_counter);
            break;

        /**
         * BVC opcode - Branch if overflow clear
         */
        case 0x50:
            branchOnCondition(pFlag::OVERFLOW, 0);
            break;

        /**
         * CLI opcode - Clear interrupt flag
         */
        case 0x58:
            processor_status.set(pFlag::INTERRUPT, false);
            break;

        /**
         * RTS opcode - Return from subroutine
         */
        case 0x60:
            program_counter = pullFromStack();
            break;

        /**
         * ADC opcode - add operand to accumulator with carry
         */
        case 0x61:
            preIndexedIndirectOperation(operation::ADC);
            break;

        case 0x65:
            zeroPageOperation(operation::ADC);
            break;

        case 0x69:
            immediateOperation(operation::ADC);
            break;

        case 0x6D:
            absoluteOperation(operation::ADC);
            break;

        case 0x71:
            postIndexedIndirectOperation(operation::ADC);
            break;

        case 0x75:
            zeroPageIndexedOperation(operation::ADC, X);
            break;

        case 0x79:
            indexedOperation(operation::ADC, Y);
            break;

        case 0x7D:
            indexedOperation(operation::ADC, X);
            break;

        /**
         * ROR opcode - rotate operand or accumulator right
         */
        case 0x66:
            zeroPageOperation(operation::ROR);
            break;

        case 0x6A:
            accumulatorOperation(operation::ROR);
            break;

        case 0x6E:
            absoluteOperation(operation::ROR);
            break;

        case 0x76:
            zeroPageIndexedOperation(operation::ROR, X);
            break;

        case 0x7E:
            indexedOperation(operation::ROR, X);
            break;

        /**
         * PLA opcode - pull accumulator from stack
         */
        case 0x68:
            accumulator = pullFromStack();
            processor_status.set(pFlag::ZERO, accumulator == 0);
            processor_status.set(pFlag::NEGATIVE, accumulator.test(7));
            break;

        /**
         * BVS opcode - branch if overflow set
         */
        case 0x70:
            branchOnCondition(pFlag::OVERFLOW, 1);
            break;

        /**
         * SEI opcode - set interrupt disable flag
         */
        case 0x78:
            processor_status.set(pFlag::INTERRUPT);
            break;

        /**
         * STA opcode - store accumulator in memory address specified by operand
         */
        case 0x81:
            preIndexedIndirectOperationWithWrite(operation::StoreRegister, accumulator);
            break;

        case 0x85:
            zeroPageOperationWithWrite(operation::StoreRegister, accumulator);
            break;

        case 0x8D:
            absoluteOperationWithWrite(operation::StoreRegister, accumulator);
            break;

        case 0x91:
            postIndexedIndirectOperationWithWrite(operation::StoreRegister, accumulator);
            break;

        case 0x95:
            zeroPageIndexedOperationWithWrite(operation::StoreRegister, X, accumulator);
            break;
        
        case 0x99:
            indexedOperationWithWrite(operation::StoreRegister, Y, accumulator);
            break;

        case 0x9D:
            indexedOperationWithWrite(operation::StoreRegister, X, accumulator);
            break;

        /**
         * STY opcode - store Y in memory address specified by operand
         */
        case 0x84:
            zeroPageOperationWithWrite(operation::StoreRegister, Y);
            break;

        case 0x8C:
            absoluteOperationWithWrite(operation::StoreRegister, Y);
            break;

        case 0x94:
            zeroPageIndexedOperationWithWrite(operation::StoreRegister, X, Y);
            break;

        /**
         * STX opcode - store X in memory address specified by operand
         */
        case 0x86:
            zeroPageOperationWithWrite(operation::StoreRegister, X);
            break;

        case 0x8E:
            absoluteOperationWithWrite(operation::StoreRegister, X);
            break;

        case 0x96:
            zeroPageIndexedOperationWithWrite(operation::StoreRegister, Y, X);
            break;
        
        /**
         * DEY opcode - Decrement Y register
         */
        case 0x88: {
            Register8 dummy = 0;
            operation::Decrement(Y, dummy, processor_status);
            break;
        }

        /**
         * TXA opcode - Transfer X to the accumulator
         */
        case 0x8A:
            transferRegisterToAccumulator(X);
            break;

        /**
         * BCC opcode - Branch if carry flag clear
         */
        case 0x90:
            branchOnCondition(pFlag::CARRY, 0);
            break;

        /**
         * TYA opcode - Transfer Y to the accumulator
         */
        case 0x98:
            transferRegisterToAccumulator(Y);
            break;

        /**
         * TXS opcode - Transfer X to the stack pointer
         */
        case 0x9A:
            stack_pointer = static_cast<uint16_t>(X.to_ulong());
            break;
        
        /**
         * LDY opcode - Load Y register from memory
         */
        case 0xA0:
            immediateOperation(operation::LoadRegister, Y);
            break;

        case 0xA4:
            zeroPageOperation(operation::LoadRegister, Y);
            break;

        case 0xAC:
            absoluteOperation(operation::LoadRegister, Y);
            break;

        case 0xB4:
            zeroPageIndexedOperation(operation::LoadRegister, X, Y);
            break;

        case 0xBC:
            indexedOperation(operation::LoadRegister, X, Y);
            break;
        
        /** 
         * LDA opcode - Load accumulator from memory
         */
        case 0xA1:
            preIndexedIndirectOperation(operation::LoadRegister, accumulator);
            break;

        case 0xA5:
            zeroPageOperation(operation::LoadRegister, accumulator);
            break;

        case 0xA9:
            immediateOperation(operation::LoadRegister, accumulator);
            break;

        case 0xAD:
            absoluteOperation(operation::LoadRegister, accumulator);
            break;

        case 0xB1:
            postIndexedIndirectOperation(operation::LoadRegister, accumulator);
            break;

        case 0xB5:
            zeroPageIndexedOperation(operation::LoadRegister, X, accumulator);
            break;

        case 0xB9:
            indexedOperation(operation::LoadRegister, Y, accumulator);
            break;

        case 0xBD:
            indexedOperation(operation::LoadRegister, X, accumulator);
            break;

        /**
         * LDX opcode - Load X register from memory
         */
        case 0xA2:
            immediateOperation(operation::LoadRegister, X);
            break;

        case 0xA6:
            zeroPageOperation(operation::LoadRegister, X);
            break;

        case 0xAE:
            absoluteOperation(operation::LoadRegister, X);
            break;

        case 0xB6:
            zeroPageIndexedOperation(operation::LoadRegister, Y, X);
            break;

        case 0xBE:
            indexedOperation(operation::LoadRegister, Y, X);
            break;

        /**
         * TAY opcode - transfer accumulator to Y register
         */
        case 0xA8:
            transferAccumulatorToRegister(Y);
            break;

        /**
         * TAX opcode - transfer accumulator to X register
         */
        case 0xAA:
            transferAccumulatorToRegister(X);
            break;

        /**
         * BCS opcode - branch if carry flag set
         */
        case 0xB0:
            branchOnCondition(pFlag::CARRY, 1);
            break;

        /**
         * CLV opcode - clear carry flag
         */
        case 0xB8:
            setProcessorStatus(pFlag::CARRY, 0);
            break;

        /**
         * TSX opcode - transfer stack pointer to X register
         */
        case 0xBA:
            transferStackPointerToRegister(X);
            break;

        /**
         * CPY opcode - compare Y register to a value in memory
         */
        case 0xC0:
            immediateOperation(operation::CompareRegisterToMemory, Y);
            break;

        case 0xC4:
            zeroPageOperation(operation::CompareRegisterToMemory, Y);
            break;

        case 0xCC:
            absoluteOperation(operation::CompareRegisterToMemory, Y);
            break;

        /**
         * CMP opcode - compare accumulator to a value in memory
         */
        case 0xC1:
            preIndexedIndirectOperation(operation::CompareRegisterToMemory, accumulator);
            break;

        case 0xC5:
            zeroPageOperation(operation::CompareRegisterToMemory, accumulator);
            break;

        case 0xC9:
            immediateOperation(operation::CompareRegisterToMemory, accumulator);
            break;

        case 0xCD:
            absoluteOperation(operation::CompareRegisterToMemory, accumulator);
            break;

        case 0xD1:
            postIndexedIndirectOperation(operation::CompareRegisterToMemory, accumulator);
            break;

        case 0xD5:
            zeroPageIndexedOperation(operation::CompareRegisterToMemory, X, accumulator);
            break;

        case 0xD9:
            indexedOperation(operation::CompareRegisterToMemory, Y, accumulator);
            break;

        case 0xDD:
            indexedOperation(operation::CompareRegisterToMemory, X, accumulator);
            break;

        /**
         * DEC opcode - decrement value in memory
         */
        case 0xC6:
            zeroPageOperationWithWrite(operation::Decrement);
            break;

        case 0xCE:
            absoluteOperationWithWrite(operation::Decrement);
            break;

        case 0xD6:
            zeroPageIndexedOperationWithWrite(operation::Decrement, X);
            break;

        case 0xDE:
            indexedOperationWithWrite(operation::Decrement, X);
            break;

        /**
         * INY opcode - increment Y register
         */
        case 0xC8:
            Register8 dummy = 0;
            operation::Increment(Y, dummy, processor_status);
            break;

        /**
         * DEX opcode - decrement X register
         */
        case 0xCA:
            Register8 dummy = 0;
            operation::Decrement(X, dummy, processor_status);
            break;

        /**
         * BNE opcode - branch if zero flag is clear
         */
        case 0xD0:
            branchOnCondition(pFlag::ZERO, 0);
            break;

        /**
         * CLD opcode - clear decimal flag
         */
        case 0xD8:
            setProcessorStatus(pFlag::DECIMAL, 0);
            break;

        /**
         * CPX opcode - compare X register to a value in memory
         */
        case 0xE0:
            immediateOperation(operation::CompareRegisterToMemory, X);
            break;

        case 0xE4:
            zeroPageOperation(operation::CompareRegisterToMemory, X);
            break;

        case 0xEC:
            absoluteOperation(operation::CompareRegisterToMemory, X);
            break;

        /**
         * SBC opcode - subtract value in memory and inverse of carry flag from accumulator
         */
        case 0xE1:
            preIndexedIndirectOperation(operation::SBC);
            break;

        case 0xE5:
            zeroPageOperation(operation::SBC);
            break;

        case 0xE9:
            immediateOperation(operation::SBC);
            break;

        case 0xED:
            absoluteOperation(operation::SBC);
            break;

        case 0xF1:
            postIndexedIndirectOperation(operation::SBC);
            break;

        case 0xF5:
            zeroPageIndexedOperation(operation::SBC, X);
            break;

        case 0xF9:
            indexedOperation(operation::SBC, Y);
            break;

        case 0xFD:
            indexedOperation(operation::SBC, X);
            break;

        /**
         * INC opcode - increment value in memory
         */
        case 0xE6:
            zeroPageOperationWithWrite(operation::Increment);
            break;

        case 0xEE:
            absoluteOperationWithWrite(operation::Increment);
            break;

        case 0xF6:
            zeroPageIndexedOperationWithWrite(operation::Increment, X);
            break;

        case 0xFE:
            indexedOperationWithWrite(operation::Increment, X);
            break;

        /**
         * INX opcode - increment X register
         */
        case 0xE8:
            Register8 dummy = 0;
            operation::Increment(X, dummy, processor_status);
            break;

        /**
         * NOP opcode - do nothing
         */
        case 0xEA:
            break;

        /**
         * BEQ opcode - branch if zero flag is set
         */
        case 0xF0:
            branchOnCondition(pFlag::ZERO, 1);
            break;

        /**
         * SED opcode - set decimal flag
         */
        case 0xF8:
            setProcessorStatus(pFlag::DECIMAL, 1);
            break;

        default:
            throw opcodeException(opcode);
    } //opcode
}
} // namespace cpu