#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto OpDD_CMPabsX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint8_t overrun{};
				std::uint16_t address{};

				if ((registers.PC & 0xFF) > 0xFE) overrun = 1;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += static_cast<std::uint16_t>(signals.DataBus << 8);

				signals.AddressBus = address + registers.X;
				memory.Read();
				std::uint8_t result = registers.A - signals.DataBus;

				if (result >= signals.DataBus)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				if (result == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (result & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 4 + overrun;
			};

	}

}
