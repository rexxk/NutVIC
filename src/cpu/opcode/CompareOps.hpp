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

		constexpr auto OpC4_CPYzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						registers.BaseAddressHigh = 0;
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 1:
					{
						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						std::uint8_t result = registers.Y - signals.DataBus;

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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpC5_CMPzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						registers.BaseAddressHigh = 0;
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 1:
					{
						signals.AddressBus = registers.BaseAddress;
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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpC9_CMP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
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
			};

		constexpr auto OpD1_CMPindY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						registers.BaseAddressHigh = 0;
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 1:
					{
						signals.AddressBus = registers.BaseAddress;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 2:
					{
						signals.AddressBus++;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						return true;
					}

					case 3:
					{
						bool overrun{ false };

						if (static_cast<std::uint16_t>(registers.BaseAddressLow += registers.Y) & 0x100)
							overrun = true;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						if (overrun)
							return true;

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

					case 4:
					{
						registers.BaseAddressHigh++;

						signals.AddressBus = registers.BaseAddress;
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
						bool overrun{ false };
						if ((static_cast<std::uint16_t>(registers.BaseAddressLow += registers.X) & 0x100)) overrun = true;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();
						std::uint8_t result = registers.A - signals.DataBus;

						if (overrun)
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
						registers.BaseAddressHigh++;
						signals.AddressBus = registers.BaseAddress;
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

				signals.Executing = false;
				return true;
			};

	}

}
