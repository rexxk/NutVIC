#pragma once

#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <print>


namespace nvic
{

	namespace opcode
	{


		constexpr auto Op4C_JMP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint16_t address = signals.DataBus;

				signals.AddressBus = registers.PC;
				memory.Read();
				address += (signals.DataBus << 8);

				registers.PC = address;

				return 3;
			};

	}

}
