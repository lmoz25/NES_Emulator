#include <iostream>
#include <bitset>
#include <functional>

uint8_t accumulator;
std::bitset<8> processor_status;

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

inline void setProcessorStatus(pFlag flag, bool value){
        if(value){
            processor_status.set(flag);
        }
        else{
            processor_status.reset(flag);
        }
    }

inline void ORA(uint8_t& operand){
        accumulator |= operand;
        setProcessorStatus(pFlag::NEGATIVE, accumulator & 0x80);
        setProcessorStatus(pFlag::ZERO, accumulator == 0);
    }

void uprint(int to_print){
    std::cout << to_print << std::endl;
}

int main(){
    std::bitset<8> accumulator; 
    accumulator.set(0,0);
    accumulator.set(1,1);
    accumulator.set(2,0);
    accumulator.set(3,0);
    accumulator.set(4,0);
    accumulator.set(5,1);
    accumulator.set(6,1);
    accumulator.set(7,1);
    uprint(accumulator.to_ulong());
    printf("0x%x\n", accumulator.to_ulong());
    uprint(static_cast<int8_t>(accumulator.to_ulong()));

}