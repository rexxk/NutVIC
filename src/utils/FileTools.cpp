#include "utils/FileTools.h"

#include <cstring>
#include <fstream>
#include <print>
#include <string>
#include <vector>


namespace nvic
{

	auto LoadROM(std::vector<MemoryChunk>& chunks, const std::string& file, std::uint16_t address) -> void
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

		auto slot = address >> 12;
		std::uint16_t chunkStartAddress = slot * 4 * 1024;
		std::uint16_t chunkAddress = address - chunkStartAddress;

		auto chunkCount = size / (4 * 1024);
		auto chunkSize = 4 * 1024;

		std::println("ROM ({}) size: {} bytes\nChunk start address: 0x{:02x}\nChunk address: 0x{:02x}", file, size, chunkStartAddress, chunkAddress);

		for (auto i = 0; i < chunkCount; i++)
		{
			MemoryChunk newChunk{ .ReadWrite{false} };
			fs.read((char*)(newChunk.Data.data() + chunkAddress), chunkSize);

			chunks.push_back(newChunk);
		}


		fs.close();
	}


}
