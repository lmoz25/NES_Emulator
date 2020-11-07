#include "CPU_Timer.h"
#include "ThreadUtils.h"

volatile uint8_t CPU_Timer::extra_cycles = 0;

CPU_Timer::CPU_Timer(const uint8_t& cycles) {
    runner = std::make_unique<std::thread>(time, cycles);
}

CPU_Timer::~CPU_Timer() {
    runner->join();
}


void CPU_Timer::time(const uint8_t& cycles) {
    auto instruction_execution_time =
        static_cast<float>(cycles) * cpu_cycle_length_useconds;

    usleep(instruction_execution_time);

    // Sleep for extra cycles, e.g if an operation has caused memory to cross a
    // page boundary
    if(extra_cycles) {
        usleep(static_cast<float>(extra_cycles) * cpu_cycle_length_useconds);
    }

    // Wake the CPU execution thread
    thread_waker.notify_all();
}
