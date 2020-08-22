#include <iostream>
#include <fstream>
#include "CPU.h"

int main(int argc, char** argv) {
	if(argc == 1){
		std::cerr << "Usage: nes.exe path/to/rom" << std::endl;
		exit(1);
	}
	std::string gamepath(argv[1]);
    cpu::CPU cpu;
    cpu.loadROM(gamepath);

	while (true){
		try{
			cpu.processNextOpcode();
		}
		catch(opcodeException& e){
			LOG(e.what());
		}
	}
}