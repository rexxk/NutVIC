#include "memory/Memory.h"

#include "cpu/Signals.h"

#include <imgui.h>

#include <cstdint>
#include <format>
#include <string>
#include <vector>


namespace nvic
{


	Memory::Memory(Signals& signals)
		: m_Signals(signals)
	{

	}

	auto Memory::Read() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 12;
		std::uint16_t address = m_Signals.AddressBus & 0x0FFF;

		m_Signals.DataBus = m_Chunks[chunk]->Data.at(address);
	}

	auto Memory::Write() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 12;
		std::uint16_t address = m_Signals.AddressBus & 0x0FFF;

		if (m_Chunks[chunk]->ReadWrite)
			m_Chunks[chunk]->Data.at(address) = m_Signals.DataBus;
	}

	auto Memory::ReadAddress(std::uint16_t address) -> std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		address &= 0x0FFF;

		return m_Chunks[chunk]->Data.at(address);
	}

	auto Memory::Push(std::uint16_t address, std::uint8_t value) -> void
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		if (m_Chunks[chunk]->ReadWrite)
			m_Chunks[chunk]->Data.at(addr) = value;

	}

	auto Memory::Pop(std::uint16_t address) -> const std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		return m_Chunks[chunk]->Data.at(addr);
	}

	auto Memory::SetChunk(std::uint8_t chunkID, MemoryChunk& chunk) -> void
	{
		m_Chunks.at(chunkID) = &chunk;
	}

	auto Memory::SetChunk(std::uint8_t chunkID, std::vector<MemoryChunk>& chunks) -> void
	{
		auto chunkCount = 0;

		for (auto& chunk : chunks)
			m_Chunks.at(chunkID + chunkCount++) = &chunk;
	}

	auto Memory::FindChunk(std::uint16_t address) -> std::uint8_t
	{
		return address >> 12;
	}


}
