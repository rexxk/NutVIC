#include "memory/Memory.h"

#include "cpu/Signals.h"


namespace nvic
{

	Memory::Memory(Signals& signals)
		: m_Signals(signals)
	{

	}

	auto Memory::SetChunk(std::uint8_t chunkID, MemoryChunk& chunk) -> void
	{
		m_Chunks.at(chunkID) = chunk;
	}

	auto Memory::FindChunk(std::uint16_t address) -> std::uint8_t
	{
		return address >> 14;
	}

}
