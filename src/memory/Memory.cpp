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

	auto Memory::SetDefaultValues() -> void
	{
		m_Signals.AddressBus = 0x0000;
		m_Signals.DataBus = 0x2F;
		Write();

		m_Signals.AddressBus = 0x0001;
		m_Signals.DataBus = 0x37;
		Write();

		m_Signals.AddressBus = 0x0000;
		m_Signals.DataBus = 0x00;
	}

	auto Memory::Read() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 12;
		std::uint16_t address = m_Signals.AddressBus & 0x0FFF;

		m_Signals.DataBus = m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(address);
	}

	auto Memory::Write() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 12;
		std::uint16_t address = m_Signals.AddressBus & 0x0FFF;

		if (m_Chunks[chunk][m_BankSelector[chunk]]->ReadWrite)
			m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(address) = m_Signals.DataBus;
	}

	auto Memory::ReadAddress(std::uint16_t address) -> std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		address &= 0x0FFF;

		return m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(address);
	}

	auto Memory::Push(std::uint16_t address, std::uint8_t value) -> void
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		if (m_Chunks[chunk][m_BankSelector[chunk]]->ReadWrite)
			m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(addr) = value;

	}

	auto Memory::Pop(std::uint16_t address) -> const std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		return m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(addr);
	}

	auto Memory::SetChunk(std::uint8_t chunkID, std::uint8_t select, MemoryChunk& chunk) -> void
	{
		m_Chunks.at(chunkID)[select] = &chunk;
	}

	auto Memory::SetChunk(std::uint8_t chunkID, std::uint8_t select, std::vector<MemoryChunk>& chunks) -> void
	{
		auto chunkCount = 0;

		for (auto& chunk : chunks)
			m_Chunks.at(chunkID + chunkCount++)[select] = &chunk;
	}

	auto Memory::FindChunk(std::uint16_t address) -> std::uint8_t
	{
		return address >> 12;
	}

	auto Memory::SetBank(std::uint8_t bank, std::uint8_t select) -> void
	{
		m_BankSelector[bank] = select;
	}


}
