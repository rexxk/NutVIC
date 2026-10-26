#pragma once

#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"



namespace nvic
{

	namespace opcode
	{

		constexpr auto OpA2_LDX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				registers.X = signals.DataBus;

				return 2;
			};

	}

}
