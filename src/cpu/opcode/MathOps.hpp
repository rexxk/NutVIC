#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{


		constexpr auto Op65_ADCzp = [](Registers& registers, Signals& signals, Memory& memory)
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

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = registers.A + signals.DataBus + carry;

						registers.A = static_cast<std::uint8_t>(result);

						if (result & 0x100)
							registers.Flags |= Flag_Carry;
						else
							registers.Flags &= ~Flag_Carry;

						if (registers.A == 0)
							registers.Flags |= Flag_Zero;
						else
							registers.Flags &= ~Flag_Zero;

						if (registers.A & 0x80)
							registers.Flags |= Flag_Negative;
						else
							registers.Flags &= ~Flag_Negative;

						if (~(registers.A ^ signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
							registers.Flags |= Flag_Overflow;
						else
							registers.Flags &= ~Flag_Overflow;

						signals.Executing = false;
						return true;
					}
				}

				signals.Executing = false;
				return true;
			};

		constexpr auto Op69_ADC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();

				std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
				std::uint16_t result = registers.A + signals.DataBus + carry;

				registers.A = static_cast<std::uint8_t>(result);

				if (result & 0x100)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

//	SUB			if ((registers.A ^ value) & (registers.A ^ static_cast<std::uint8_t>(result) & 0x80))
				if (~(registers.A ^ signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
					registers.Flags |= Flag_Overflow;
				else
					registers.Flags &= ~Flag_Overflow;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op88_DEY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Y--;

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

		constexpr auto OpC6_DECzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.InternalRegister8 = signals.DataBus;

						return true;
					}

					case 2:
					{
						memory.Read();
						registers.InternalRegister8--;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.InternalRegister8;
						memory.Write();

						if (registers.InternalRegister8 == 0)
							registers.Flags |= Flag_Zero;
						else
							registers.Flags &= ~Flag_Zero;

						if (registers.InternalRegister8 & 0x80)
							registers.Flags |= Flag_Negative;
						else
							registers.Flags &= ~Flag_Negative;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpC8_INY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Y++;

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

		constexpr auto OpCA_DEX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.X--;

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

		constexpr auto OpE6_INCzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.InternalRegister8 = signals.DataBus;

						return true;
					}

					case 2:
					{
						memory.Read();
						registers.InternalRegister8++;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.InternalRegister8;
						memory.Write();

						if (registers.InternalRegister8 == 0)
							registers.Flags |= Flag_Zero;
						else
							registers.Flags &= ~Flag_Zero;

						if (registers.InternalRegister8 & 0x80)
							registers.Flags |= Flag_Negative;
						else
							registers.Flags &= ~Flag_Negative;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpE8_INX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.X++;

				if (registers.X == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.X & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				signals.Executing = false;
				return true;
			};

		constexpr auto OpE5_SBCzp = [](Registers& registers, Signals& signals, Memory& memory)
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

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = registers.A - signals.DataBus - carry;

						registers.A = static_cast<std::uint8_t>(result);

						if (result & 0x100)
							registers.Flags |= Flag_Carry;
						else
							registers.Flags &= ~Flag_Carry;

						if (registers.A == 0)
							registers.Flags |= Flag_Zero;
						else
							registers.Flags &= ~Flag_Zero;

						if (registers.A & 0x80)
							registers.Flags |= Flag_Negative;
						else
							registers.Flags &= ~Flag_Negative;

						if ((registers.A ^ signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
							registers.Flags |= Flag_Overflow;
						else
							registers.Flags &= ~Flag_Overflow;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpE9_SBC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();

				std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
				std::uint16_t result = registers.A - signals.DataBus - carry;

				registers.A = static_cast<std::uint8_t>(result);

				if (result & 0x100)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (registers.A & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				if ((registers.A ^ signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
					registers.Flags |= Flag_Overflow;
				else
					registers.Flags &= ~Flag_Overflow;

				signals.Executing = false;
				return true;
			};
	}

}
