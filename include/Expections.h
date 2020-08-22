#pragma once

#include <exception>
#include <cstring>
#include <stdint-gcc.h>
#include <cstdio>

class opcodeException : public std::exception {
public:
    opcodeException(uint8_t opcode) : opcode_(opcode) {}

    char * what () {
        char message[strlen(format) + 2];
        sprintf(message, format, opcode_);
        return message;
    }

private:
    static constexpr char* format = "Unexpected opcode 0x%x";
    uint8_t opcode_;
};