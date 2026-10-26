#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op84_STYzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = address;
				signals.DataBus = registers.Y;
				memory.Write();

				return 3;
			};

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

		constexpr auto Op86_STXzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = address;
				signals.DataBus = registers.X;
				memory.Write();

				return 3;
			};

		constexpr auto Op8C_STYabs = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address;
				signals.DataBus = registers.Y;
				memory.Write();

				return 4;
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

		constexpr auto Op91_STAindY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};
				std::uint16_t zeropageAddress{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				zeropageAddress = signals.DataBus;

				signals.AddressBus = zeropageAddress;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address + registers.Y;
				signals.DataBus = registers.A;
				memory.Write();

				return 6;
			};

		constexpr auto Op94_STYzpX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = (address + registers.X) & 0xFF;
				signals.DataBus = registers.Y;
				memory.Write();

				return 4;
			};

		constexpr auto Op95_STAzpX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = (address + registers.X) & 0xFF;
				signals.DataBus = registers.A;
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

		constexpr auto Op9D_STAabsX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address + registers.X;
				signals.DataBus = registers.A;
				memory.Write();

				return 5;
			};

	}

}
