#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op78_SEI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags |= Flag_Interrupt;

				return 2;
			};

		constexpr auto OpD8_CLD = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Decimal;

				return 2;
			};
	}

}
