#pragma once

#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op9A_TXS = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.StackPointer = registers.X;

				return 2;
			};


	}

}
