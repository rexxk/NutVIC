#pragma once

#include "cpu/Registers.h"


namespace nvic
{

	class CPUPanel
	{
	public:
		CPUPanel(Registers& registers);

		auto Draw() -> void;

	private:
		Registers& m_Registers;
	};


}
