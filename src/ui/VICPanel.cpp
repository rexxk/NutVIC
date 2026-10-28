#include "ui/VICPanel.h"


#include <imgui.h>


namespace nvic
{

	VICPanel::VICPanel(VIC_II& vic)
		: m_VIC(vic)
	{

	}

	auto VICPanel::Draw() -> void
	{
		ImGui::Begin("VIC-II");

		ImGui::Text("Memory bank: %02x", m_VIC.GetRAMSelect());
		ImGui::Text("Base address: %02x", m_VIC.GetBaseAddress());

		ImGui::End();
	}

}
