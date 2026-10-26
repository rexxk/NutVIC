#pragma once

#include "memory/Memory.h"

#include <cstring>
#include <string>
#include <vector>


namespace nvic
{

	auto LoadROM(std::vector<MemoryChunk>& chunks, const std::string& file, std::uint8_t slot, std::uint16_t address) -> void;


}
