#pragma once

#include "memory/Memory.h"

#include <cstring>
#include <string>


namespace nvic
{

	auto LoadROM(MemoryChunk& lowROM, const std::string& file, std::uint8_t slot, std::uint16_t address) -> void;


}
