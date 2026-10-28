#include "ui/CPUPanel.h"

#include "cpu/Flags.h"

#include <imgui.h>

#include <format>
#include <string>


namespace nvic
{

	CPUPanel::CPUPanel(Registers& registers, Signals& signals)
		: m_Registers(registers), m_Signals(signals)
	{

	}

	auto CPUPanel::Draw() -> void
	{
		ImGui::Begin("CPU Panel");

		ImGui::Text("Register info");

		ImGui::Separator();

		ImGui::Text("A: %02x", m_Registers.A);
		ImGui::Text("X: %02x", m_Registers.X);
		ImGui::Text("Y: %02x", m_Registers.Y);
		ImGui::Text("SP: %02x", m_Registers.StackPointer);
		ImGui::Text("PC: %04x", m_Registers.PC);

		ImGui::Separator();

		ImGui::Text("Flags");

		std::string str;

		if (m_Registers.Flags & Flag_Negative) str += "N"; else str += "n";
		if (m_Registers.Flags & Flag_Overflow) str += "V"; else str += "v";
		str += "-";
		if (m_Registers.Flags & Flag_Break) str += "B"; else str += "b";
		if (m_Registers.Flags & Flag_Decimal) str += "D"; else str += "d";
		if (m_Registers.Flags & Flag_Interrupt) str += "I"; else str += "i";
		if (m_Registers.Flags & Flag_Zero) str += "Z"; else str += "z";
		if (m_Registers.Flags & Flag_Carry) str += "C"; else str += "c";

		ImGui::Text("%s", str.c_str());

		ImGui::Separator();

		ImGui::Text("Address bus: %04x", m_Signals.AddressBus);
		ImGui::Text("Data bus: %02x", m_Signals.DataBus);

		ImGui::Separator();

		ImGui::Text("Second last operand: %02x", m_Registers.SecondLastOperand);
		ImGui::Text("Last operand: %02x", m_Registers.LastOperand);
		ImGui::Text("Active operand: %02x", m_Registers.ActiveOperand);

		ImGui::End();
	}

}
