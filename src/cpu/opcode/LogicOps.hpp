#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op09_ORA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint8_t value = signals.DataBus;

				registers.A = registers.A | value;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 2;
			};

		constexpr auto Op0D_ORAabs = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address;
				memory.Read();
				std::uint8_t value = signals.DataBus;

				registers.A = registers.A | value;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 4;
			};

		constexpr auto Op29_AND = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint8_t value = signals.DataBus;

				registers.A = registers.A & value;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 2;
			};

	}

}
