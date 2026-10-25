#pragma once

#include "cpu/Registers.h"
#include "cpu/Signals.h"


namespace nvic
{

	class CPUPanel
	{
	public:
		CPUPanel(Registers& registers, Signals& signals);

		auto Draw() -> void;

	private:
		Registers& m_Registers;
		Signals& m_Signals;
	};


}
