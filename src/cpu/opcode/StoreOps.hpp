#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op85_STAzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = address;
				signals.DataBus = registers.A;
				memory.Write();

				return 3;
			};

		constexpr auto Op8D_STAabs = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address;
				signals.DataBus = registers.A;
				memory.Write();

				return 4;
			};

		constexpr auto Op8E_STXabs = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address;
				signals.DataBus = registers.X;
				memory.Write();

				return 4;
			};

		constexpr auto Op99_STAabsY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address + registers.Y;
				signals.DataBus = registers.A;
				memory.Write();

				return 5;
			};
	}

}
