#pragma once

#include "cpu/Flags.h"
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

		constexpr auto OpA8_TAY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Y = registers.A;

				if (registers.Y == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.Y & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 2;
			};

	}

}
