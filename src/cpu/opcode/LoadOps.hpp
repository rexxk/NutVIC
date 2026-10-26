#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"



namespace nvic
{

	namespace opcode
	{

		constexpr auto OpA0_LDY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				registers.Y = signals.DataBus;

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

		constexpr auto OpA2_LDX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				registers.X = signals.DataBus;

				if (registers.X == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.X & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 2;
			};

		constexpr auto OpA4_LDYzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t zeropageAddress{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				zeropageAddress = signals.DataBus;

				signals.AddressBus = zeropageAddress;
				memory.Read();
				registers.Y = signals.DataBus;

				if (registers.Y == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.Y & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 3;
			};

		constexpr auto OpA9_LDA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				registers.A = signals.DataBus;

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

		constexpr auto OpAD_LDAabs = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += static_cast<std::uint16_t>(signals.DataBus << 8);

				signals.AddressBus = address;
				memory.Read();
				registers.A = signals.DataBus;

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

		constexpr auto OpB1_LDAindY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint8_t overrun{};
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
				memory.Read();
				registers.A = signals.DataBus;

				if ((address >> 8) != ((address + registers.Y) >> 8))
					overrun = 1;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 5 + overrun;
			};

		constexpr auto OpB5_LDAzpX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t zeropageAddress{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				zeropageAddress = signals.DataBus;

				signals.AddressBus = (zeropageAddress + registers.X) & 0xFF;
				memory.Read();
				registers.A = signals.DataBus;

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

		constexpr auto OpB9_LDAabsY = [](Registers& registers, Signals& signals, Memory& memory)
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

				signals.AddressBus = address + registers.Y;
				memory.Read();
				registers.A = signals.DataBus;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 4 + overrun;
			};

		constexpr auto OpBD_LDAabsX = [](Registers& registers, Signals& signals, Memory& memory)
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
				registers.A = signals.DataBus;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 4 + overrun;
			};
	}

}
