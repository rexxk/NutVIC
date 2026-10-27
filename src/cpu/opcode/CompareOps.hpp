#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto OpC1_CMPidxind = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};
				std::uint8_t zeropageValue{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				zeropageValue = signals.DataBus + registers.X;

				signals.AddressBus = static_cast<std::uint16_t>(zeropageValue);
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = static_cast<std::uint16_t>(zeropageValue) + 1;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address;
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

				return 6;
			};

		constexpr auto OpD1_CMPindY = [](Registers& registers, Signals& signals, Memory& memory)
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

				if ((address >> 8) != ((address + registers.Y) >> 8))
					overrun = 1;

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

				return 5 + overrun;
			};

		constexpr auto OpDD_CMPabsX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}
					case 1:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						return true;
					}
					case 2:
					{
						signals.AddressBus = registers.BaseAddressHigh << 8 + registers.BaseAddressLow + registers.X;
						memory.Read();
						std::uint8_t result = registers.A - signals.DataBus;

						if (static_cast<std::uint16_t>(registers.BaseAddressLow + registers.X) & 0x100)
						{
							return true;
						}

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

						signals.Executing = false;

						return true;
					}
					case 3:
					{
						signals.AddressBus = (registers.BaseAddressHigh + 1) << 8 + registers.BaseAddressLow + registers.X;
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

						signals.Executing = false;

						return true;
					}
				}

				return true;
			};

		constexpr auto OpE0_CPX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();

				std::uint8_t result = registers.X - signals.DataBus;

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

				return 2;
			};
	}

}
