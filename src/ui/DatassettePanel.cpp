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
			m_Datassette.OpenTapeImage("data/tapes/Last_Ninja_2.tap");
		}

		ImGui::Checkbox("Sense", &m_Datassette.m_Signals.Datassette_SENSE);

		ImGui::End();
	}

}
