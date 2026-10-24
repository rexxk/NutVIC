#include "memory/Memory.h"

#include "cpu/Signals.h"

#include <imgui.h>

#include <cstdint>
#include <format>
#include <string>


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

		m_Signals.DataBus = m_Chunks[chunk]->Data.at(address);
	}

	auto Memory::Write() -> void
	{
		std::uint8_t chunk = m_Signals.AddressBus >> 14;
		std::uint16_t address = m_Signals.AddressBus & 0x3FFF;

		if (m_Chunks[chunk]->ReadWrite)
			m_Chunks[chunk]->Data.at(address) = m_Signals.DataBus;
	}

	auto Memory::SetChunk(std::uint8_t chunkID, MemoryChunk& chunk) -> void
	{
		m_Chunks.at(chunkID) = &chunk;
	}

	auto Memory::FindChunk(std::uint16_t address) -> std::uint8_t
	{
		return address >> 14;
	}


	auto Memory::ViewMemory() -> void
	{
		ImGui::Begin("Memory page");

			if (ImGui::InputInt("Page", &m_PageNumber, 1, 100, ImGuiInputTextFlags_CharsHexadecimal))
			{
				if (m_PageNumber < 0) m_PageNumber = 0;
				if (m_PageNumber > 0xFF) m_PageNumber = 0xFF;
			}

			ImGui::Separator();

			auto chunk = m_PageNumber >> 6;
			auto chunkStartAddress = chunk * 16 * 1024;
			auto chunkAddress = (m_PageNumber * 0x100) - chunkStartAddress;

			std::uint16_t count{};

			for (auto row = 0; row < 16; row++)
			{
				std::string str = std::format("{:04x} : ", row * 16 + m_PageNumber * 0x100);

				for (auto col = 0; col < 16; col++)
				{
					str += std::format("{:02x} ", m_Chunks.at(chunk)->Data[row * 16 + chunkAddress + col]);
				}

				ImGui::Text("%s", str.c_str());
			}


		ImGui::End();
	}

}
