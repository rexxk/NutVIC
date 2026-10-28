#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op18_CLC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Carry;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op58_CLI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Interrupt;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op78_SEI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags |= Flag_Interrupt;

				signals.Executing = false;
				return true;
			};

		constexpr auto OpD8_CLD = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Decimal;

				signals.Executing = false;
				return true;
			};
	}

}
