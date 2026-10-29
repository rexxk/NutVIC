#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op8A_TXA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.A = registers.X;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op98_TYA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.A = registers.Y;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op9A_TXS = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.ExecutionCycle++;

				registers.StackPointer = registers.X;

				signals.Executing = false;
				return true;
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

				signals.Executing = false;
				return 2;
			};

		constexpr auto OpAA_TAX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.X = registers.A;

				if (registers.X == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.X & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				signals.Executing = false;
				return 2;
			};

		constexpr auto OpBA_TSX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.X = registers.StackPointer;

				if (registers.X == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.X & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				signals.Executing = false;
				return 2;
			};

	}

}
