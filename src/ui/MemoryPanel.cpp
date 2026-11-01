#include "ui/MemoryPanel.h"

#include <imgui.h>

#include <format>
#include <string>


namespace nvic
{

	MemoryPanel::MemoryPanel(Memory& memory, Registers& registers)
		: m_Memory(memory), m_Registers(registers)
	{

	}

	auto MemoryPanel::Draw() -> void
	{
		if (m_TraceProgramCounter)
		{
			m_PageNumber = m_Registers.PC >> 8;
			m_PageAddress = m_Registers.PC & 0xFF;
		}

		auto chunks = m_Memory.GetChunks();
		auto chunk = m_PageNumber >> 4;
		auto chunkStartAddress = chunk * 4 * 1024;
		auto chunkAddress = (m_PageNumber * 0x100) - chunkStartAddress;

		ImGui::Begin("Memory page");

		if (ImGui::InputInt("Page", &m_PageNumber, 1, 100, ImGuiInputTextFlags_CharsHexadecimal))
		{
			if (m_PageNumber < 0) m_PageNumber = 0;
			if (m_PageNumber > 0xFF) m_PageNumber = 0xFF;
		}

		if (!m_TraceProgramCounter)
		{
			if (ImGui::InputInt("Bank", &m_BankNumber, 1, 1))
			{
				if (m_BankNumber < 0) m_BankNumber = 0;
				if (m_BankNumber > 2) m_BankNumber = 2;

				if (m_Memory.m_Chunks[chunk][m_BankNumber] == nullptr)
					m_BankNumber--;
			}
		}

		ImGui::Separator();

		ImGui::Checkbox("Trace PC", &m_TraceProgramCounter);

		ImGui::Separator();


		std::uint16_t count{};
		
		bool sameLine{ false };

		auto activeBank = m_TraceProgramCounter ? m_Memory.m_ActiveBanks[chunk] : m_BankNumber;

		for (auto row = 0; row < 16; row++)
		{
			std::string str = std::format("{:04x} : ", row * 16 + m_PageNumber * 0x100);

			for (auto col = 0; col < 16; col++)
			{
				if (m_TraceProgramCounter && ((col + row * 16) == m_PageAddress))
				{
					if (m_PageAddress != 0)
						str = str.substr(0, str.size() - 1);

					ImGui::Text("%s", str.c_str());

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.1f, 0.9f, 0.1f, 1.0f });
					str = std::format("{:02x}", chunks.at(chunk)[activeBank]->Data[row * 16 + chunkAddress + col]);
					ImGui::Text("%s", str.c_str());
					ImGui::PopStyleColor();

					str = "";

					if (m_PageAddress != 15)
						sameLine = true;
				}
				else
					str += std::format("{:02x} ", chunks.at(chunk)[activeBank]->Data[row * 16 + chunkAddress + col]);
			}

			if (sameLine)
			{
				ImGui::SameLine();
				sameLine = false;
			}

			ImGui::Text("%s", str.c_str());
		}


		ImGui::End();

	}

}
