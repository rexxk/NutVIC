#include "ui/DatassettePanel.h"
#include "devices/Datassette.h"

#include <imgui.h>


namespace nvic
{

	DatassettePanel::DatassettePanel(Datassette& datassette)
		: m_Datassette(datassette)
	{

	}

	auto DatassettePanel::Draw() -> void
	{
		ImGui::Begin("Tape 1530");

		ImGui::Text("Datassette panel");


		if (m_Datassette.m_CassetteInserted)
		{
			if (ImGui::Button("Eject"))
			{
				m_Datassette.m_CassetteInserted = false;
			}

			ImGui::SameLine();

			ImGui::Text("%s", m_Datassette.m_ImageName.c_str());
		}

		else
		{
			if (ImGui::Button("Insert"))
			{
				m_Datassette.OpenTapeImage("data/tapes/Pitstop_II.tap");
//				m_Datassette.OpenTapeImage("data/tapes/Last_Ninja_2.tap");
//				m_Datassette.OpenTapeImage("data/tapes/Olympic_Skier.tap");

				m_Datassette.m_CassetteInserted = true;
			}
		}


		if (m_Datassette.m_CassetteInserted)
		{
//			ImGui::Checkbox("Play", &m_Datassette.m_Signals.Datassette_PLAY);

			if (ImGui::Button("Play"))
			{
				m_Datassette.m_Signals.Datassette_PLAY = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Stop"))
			{
				m_Datassette.m_Signals.Datassette_PLAY = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Rew"))
			{
				m_Datassette.m_FilePointer = 0;
			}

			ImGui::SameLine();

			ImGui::Text("Status: %s", m_Datassette.m_Signals.Datassette_PLAY ? "PLAY" : "STOP");

			ImGui::SameLine();
			ImGui::Text("Pointer: %lu  (%lu)", m_Datassette.GetFilePointer(), m_Datassette.m_Image.DataSize);

			m_Datassette.m_Signals.Datassette_SENSE = !m_Datassette.m_Signals.Datassette_PLAY;
		}

		ImGui::End();
	}

}
