#include "CPU.h"
#include "CPU_Timer.h"
#include "ThreadUtils.h"

namespace cpu {
CPU::CPU()
    : X(0), Y(0), accumulator(0), processor_status(0), stack_pointer(STACK_START), program_counter(0){}

void CPU::processNextOpcode(){
    uint8_t opcode = *memory_map.read(program_counter);

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
    // TODO: Getting a pointer and then passing a reference might be dumb
    operation_tuple.op(*this, *operand);
}

std::shared_ptr<std::bitset<8>> CPU::getOperandFromMemory(const AddressingMode& addressing_mode) {
    std::shared_ptr<std::bitset<8>> operand;
    switch (addressing_mode)
    {
    case AddressingMode::IMMEDIATE:
        operand = std::make_shared<std::bitset<8>>(
            memory_map.immediateRead(program_counter));
        break;
    
    default:
        break;
    }

    return operand;
}
} // cpu::