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

		ImGui::Text("Mode: %s", m_VIC.m_BitmapMode ? "Bitmap" : "Text");
		ImGui::Text("Screen mode: %d", m_VIC.m_ScreenMode);

		ImGui::End();
	}

}
