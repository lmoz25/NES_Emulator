#pragma once

#include <unistd.h>
#include <thread>
#include <memory>

/**
* Class to manage CPU timing.
* CPU execution thread notifies timing thread when it starts to process a new
* opcode, timing thread then sleeps until instruction should finish according to
* 6502 clock speed, then notifies  CPU execution thread, telling it to process new
* opcode.
*
* A 6502 CPU runs at 1-2MHz, so, unless this emulator is run on a toaster,
* the timing thread will not wake up before the execution thread finishes processing
**/

class CPU_Timer {
public:
    CPU_Timer(const uint8_t& cycles);
    ~CPU_Timer();

    // Average cycle length of 6502 CPU, obviously variable irl
    static constexpr float cpu_cycle_length_useconds = 0.5;
    static volatile uint8_t extra_cycles;


private:
    // Main loop
    static void time(const uint8_t& cycles);

    std::unique_ptr<std::thread> runner;
};