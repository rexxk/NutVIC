#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op0A_ASL = [](Registers& registers, Signals& signals, Memory& memory)
			{
				if (registers.A & 0x80)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				registers.A <<= 1;

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

		constexpr auto Op2A_ROL = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint8_t oldCarry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
				
				if (registers.A & 0x80)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				registers.A <<= 1;
				registers.A += oldCarry;

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

	}

}
