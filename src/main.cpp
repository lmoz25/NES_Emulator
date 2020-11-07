#include <iostream>
#include <fstream>
#include "CPU.h"


void loadROM(std::string& path) {
	memory::MemoryMap memory_map;

    std::ifstream gamefile;
    gamefile.open(path.c_str());
    uint8_t ch = gamefile.get();
    int i = ROM_START;
    // Rest of memory is rom data
    while (gamefile.good() && i < MEMORY_SIZE){
        memory_map.write(i, ch);
        ch = gamefile.get();
    }
    gamefile.close();
}

int main(int argc, char** argv) {
	if(argc == 1){
		std::cerr << "Usage: nes.exe path/to/rom" << std::endl;
		exit(1);
	}
	std::string gamepath(argv[1]);
    cpu::CPU cpu;
    loadROM(gamepath);

	while (true){
		try{
			cpu.processNextOpcode();
		}
		catch(opcodeException& e){
			LOG(e.what());
		}
	}
}