#include "CPU_Timer.h"
#include "ThreadUtils.h"

CPU_Timer::CPU_Timer(uint8_t& cycles): cycles(cycles){
    runner = std::make_unique<std::thread>(time());
}

CPU_Timer::~CPU_Timer() {
    runner.join();
}


void CPU_Timer::time() {
    auto instruction_exectution_time =
        static_cast<float>(cycles) * cycle_length_useconds;

    usleep(instruction_execution_time);

    // Wake the CPU execution thread
    thread_waker.notify_all();
}
