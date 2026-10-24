#include "utils/FileTools.h"

#include <cstring>
#include <fstream>
#include <print>
#include <string>


namespace nvic
{

	auto LoadROM(MemoryChunk& chunk, const std::string& file, std::uint8_t slot, std::uint16_t address) -> void
	{
		std::ifstream fs(file, std::ios::in | std::ios::binary);

		if (!fs.is_open())
		{
			std::println("Error: unable to open file {}", file);
			return;
		}

		fs.seekg(0, fs.end);
		std::size_t size = fs.tellg();
		fs.seekg(0, fs.beg);

		std::uint16_t chunkStartAddress = slot * 16 * 1024;
		std::uint16_t chunkAddress = address - chunkStartAddress;

		std::println("ROM ({}) size: {} bytes\nChunk start address: 0x{:02x}\nChunk address: 0x{:02x}", file, size, chunkStartAddress, chunkAddress);

		fs.read((char*)(chunk.Data.data() + chunkAddress), size);

		fs.close();
	}


}
