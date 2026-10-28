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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpA2_LDX = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.ExecutionCycle++;

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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpA4_LDYzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.Y = signals.DataBus;

						if (registers.Y == 0)
							registers.Flags |= Flag_Zero;
						else
							registers.Flags &= ~Flag_Zero;

						if (registers.Y & 0x80)
							registers.Flags |= Flag_Negative;
						else
							registers.Flags &= ~Flag_Negative;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpA5_LDAzp = [](Registers& registers, Signals& signals, Memory& memory)
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
					registers.A = signals.DataBus;

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
				}
				}

				return true;
			};

		constexpr auto OpA6_LDXzp = [](Registers& registers, Signals& signals, Memory& memory)
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
					registers.X = signals.DataBus;

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
				}
				}

				return true;
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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpAD_LDAabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB1_LDAindY = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.A = signals.DataBus;

						if (overrun)
							return true;

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
					}

					case 4:
					{
						registers.BaseAddressHigh++;

						signals.AddressBus = registers.BaseAddress;
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


						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB4_LDYzpX = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.BaseAddressLow += registers.X;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB5_LDAzpX = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.BaseAddressLow += registers.X;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB6_LDXzpY = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.BaseAddressLow += registers.Y;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB9_LDAabsY = [](Registers& registers, Signals& signals, Memory& memory)
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
						if ((static_cast<std::uint16_t>(registers.BaseAddressLow += registers.Y) & 0x100)) overrun = true;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();
						registers.A = signals.DataBus;

						if (overrun)
						{
							return true;
						}

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
					}
					case 3:
					{
						registers.BaseAddressHigh++;
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpBD_LDAabsX = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.A = signals.DataBus;

						if (overrun)
						{
							return true;
						}

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
					}
					case 3:
					{
						registers.BaseAddressHigh++;
						signals.AddressBus = registers.BaseAddress;
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

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};
	}

}
