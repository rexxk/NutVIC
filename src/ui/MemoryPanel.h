#pragma once

#include "cpu/Registers.h"
#include "memory/Memory.h"



namespace nvic
{


	class MemoryPanel
	{
	public:
		MemoryPanel(Memory& memory, Registers& registers);

		auto Draw() -> void;

	private:
		Memory& m_Memory;
		Registers& m_Registers;

		bool m_TraceProgramCounter{ true };
		int m_PageNumber{ 0xFF };

		std::uint8_t m_PageAddress{};
	};


}
