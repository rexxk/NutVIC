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

		if (ImGui::Button("Load image"))
		{
//			m_Datassette.OpenTapeImage("data/tapes/Last_Ninja_2.tap");
//			m_Datassette.OpenTapeImage("data/tapes/Olympic_Skier.tap");
			m_Datassette.OpenTapeImage("data/tapes/Pitstop_II.tap");
			m_Datassette.m_CassetteInserted = true;
		}

		if (m_Datassette.m_CassetteInserted)
		{
			ImGui::Checkbox("Play", &m_Datassette.m_Signals.Datassette_PLAY);

			m_Datassette.m_Signals.Datassette_SENSE = !m_Datassette.m_Signals.Datassette_PLAY;

			ImGui::SameLine();
			ImGui::Text("Pointer: %lu", m_Datassette.GetFilePointer());
		}

		ImGui::End();
	}

}
