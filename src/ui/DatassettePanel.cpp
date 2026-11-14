#include "ui/DatassettePanel.h"

#include <imgui.h>


namespace nvic
{

	DatassettePanel::DatassettePanel()
	{

	}

	auto DatassettePanel::Draw() -> void
	{
		ImGui::Begin("Tape 1530");

		ImGui::Text("Datassette panel");

		ImGui::End();
	}

}
