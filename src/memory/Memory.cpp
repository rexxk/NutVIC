#include "memory/Memory.h"

#include "cpu/Signals.h"


namespace nvic
{

	Memory::Memory(Signals& signals)
		: m_Signals(signals)
	{

	}

	auto Memory::Read() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 14;
		std::uint16_t address = m_Signals.AddressBus & 0x3FFF;

		m_Signals.DataBus = m_Chunks[chunk].Data.at(address);
	}

	auto Memory::Write() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 14;
		std::uint16_t address = m_Signals.AddressBus & 0x3FFF;

		if (m_Chunks[chunk].ReadWrite)
			m_Chunks[chunk].Data.at(address) = m_Signals.DataBus;
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
