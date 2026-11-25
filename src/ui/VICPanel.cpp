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

		ImGui::Text("Screen mode: %d", m_VIC.m_ScreenMode);

		ImGui::SliderInt("Row adjust", &m_VIC.m_RowAdjust, 50, 70);
		ImGui::Text("Raster interrupt: %04x", m_VIC.m_RasterInterruptLine);

		ImGui::End();
	}

}
