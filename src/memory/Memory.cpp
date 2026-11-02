#include "memory/Memory.h"

#include "cpu/Signals.h"
#include "cpu/Registers.h"

#include <imgui.h>

#include <cstdint>
#include <format>
#include <string>
#include <vector>


namespace nvic
{


	Memory::Memory(PLA& pla, Signals& signals, Registers& registers)
		: m_PLA(pla), m_Signals(signals), m_Registers(registers)
	{
		for (auto chunk = 0; chunk < 16; chunk++)
			for (auto bank = 0; bank < 3; bank++)
				m_Chunks[chunk][bank] = nullptr;

		m_ActiveBanks[0xA] = 1;
		m_ActiveBanks[0xB] = 1;
		m_ActiveBanks[0xE] = 1;
		m_ActiveBanks[0xF] = 1;
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

		m_BankSelector = m_PLA.GetBankSelector(true);
		m_ActiveBanks[chunk] = m_BankSelector;

		m_Signals.DataBus = m_Chunks[chunk][m_BankSelector]->Data.at(address);
	}

	auto Memory::Write() -> void
	{
//		if (m_Signals.AddressBus == 0x0001)
//			m_Signals.Break = true;

		std::uint8_t chunk = m_Signals.AddressBus >> 12;
		std::uint16_t address = m_Signals.AddressBus & 0x0FFF;

		m_BankSelector = m_PLA.GetBankSelector(false);
		m_ActiveBanks[chunk] = m_BankSelector;
//		if (m_Chunks[chunk][m_BankSelector[chunk]]->ReadWrite)
//			m_Chunks[chunk][m_BankSelector[chunk]]->Data.at(address) = m_Signals.DataBus;
		m_Chunks[chunk][m_BankSelector]->Data.at(address) = m_Signals.DataBus;

		// Is there a need to set latches in IO?
		if (m_Signals.WriteToIO)
		{
			if (address == 0xC04) m_Registers.CIA1_TimerA_LowByte_Latch = m_Signals.DataBus;
			if (address == 0xC05) m_Registers.CIA1_TimerA_HighByte_Latch = m_Signals.DataBus;
			if (address == 0xC06) m_Registers.CIA1_TimerB_LowByte_Latch = m_Signals.DataBus;
			if (address == 0xC07) m_Registers.CIA1_TimerB_HighByte_Latch = m_Signals.DataBus;

			if (address == 0xD04) m_Registers.CIA2_TimerA_LowByte_Latch = m_Signals.DataBus;
			if (address == 0xD05) m_Registers.CIA2_TimerA_HighByte_Latch = m_Signals.DataBus;
			if (address == 0xD06) m_Registers.CIA2_TimerB_LowByte_Latch = m_Signals.DataBus;
			if (address == 0xD07) m_Registers.CIA2_TimerB_HighByte_Latch = m_Signals.DataBus;


			m_Signals.WriteToIO = false;
		}
	}

	auto Memory::ReadAddress(std::uint16_t address) -> std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		address &= 0x0FFF;

		m_Signals.AddressBus = address;
		m_BankSelector = m_PLA.GetBankSelector(true);

		return m_Chunks[chunk][m_BankSelector]->Data.at(address);
	}

	auto Memory::Push(std::uint16_t address, std::uint8_t value) -> void
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		m_BankSelector = m_PLA.GetBankSelector(false);
		m_ActiveBanks[chunk] = m_BankSelector;

//		if (m_Chunks[chunk][m_BankSelector[chunk]]->ReadWrite)
		m_Chunks[chunk][m_BankSelector]->Data.at(addr) = value;

	}

	auto Memory::Pop(std::uint16_t address) -> const std::uint8_t
	{
		std::uint8_t chunk = address >> 12;
		std::uint16_t addr = address & 0x0FFF;

		m_BankSelector = m_PLA.GetBankSelector(true);
		m_ActiveBanks[chunk] = m_BankSelector;

		return m_Chunks[chunk][m_BankSelector]->Data.at(addr);
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


}
