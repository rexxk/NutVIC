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
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(signals.DataBus) + static_cast<std::uint8_t>(carry);

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
				std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(signals.DataBus) + static_cast<std::uint8_t>(carry);

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

		constexpr auto Op79_ADCabsY = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.InternalRegister = static_cast<std::uint16_t>(registers.BaseAddressLow) + registers.Y;
						registers.BaseAddressLow = registers.InternalRegister & 0xFF;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						if (registers.InternalRegister & 0x100)
							return true;

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(signals.DataBus) + static_cast<std::uint8_t>(carry);

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

					case 3:
					{
						registers.BaseAddressHigh++;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(signals.DataBus) + static_cast<std::uint8_t>(carry);

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

		constexpr auto OpCE_DECabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.InternalRegister8 = signals.DataBus;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						memory.Write();
						registers.InternalRegister8--;

						return true;
					}

					case 4:
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
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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

//						if (~(registers.A ^ ~signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
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

		constexpr auto OpE9_SBC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();

				std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
				std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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

//				if (~(registers.A ^ ~signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
				if ((registers.A ^ signals.DataBus) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
					registers.Flags |= Flag_Overflow;
				else
					registers.Flags &= ~Flag_Overflow;

				signals.Executing = false;
				return true;
			};

		constexpr auto OpEE_INCabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						registers.InternalRegister8 = signals.DataBus;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						memory.Write();
						registers.InternalRegister8++;

						return true;
					}

					case 4:
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

				signals.Executing = false;
				return true;
			};

		constexpr auto OpF1_SBCindY = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
				case 0:
				{
					registers.BaseAddress = 0;
					signals.AddressBus = registers.PC++;
					memory.Read();
					registers.BaseAddressLow = signals.DataBus;

					return true;
				}
				case 1:
				{
					registers.InternalRegister = registers.BaseAddress;
					signals.AddressBus = registers.InternalRegister;
					memory.Read();
					registers.BaseAddressLow = signals.DataBus;

					return true;
				}
				case 2:
				{
					signals.AddressBus = (registers.InternalRegister + 1) & 0xFF;
					memory.Read();
					registers.BaseAddressHigh = signals.DataBus;
					registers.BaseAddressLow += registers.Y;

					return true;
				}
				case 3:
				{
					bool overrun{ false };

					registers.InternalRegister = registers.BaseAddressLow + registers.Y;

					if (registers.InternalRegister & 0x100) overrun = true;

					signals.AddressBus = registers.BaseAddress;
					memory.Read();

					if (overrun)
					{
						return true;
					}

					std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
					std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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
				case 4:
				{
					registers.BaseAddressHigh++;
					signals.AddressBus = registers.BaseAddress;
					memory.Read();

					std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
					std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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

			constexpr auto OpF5_SBCzpX = [](Registers& registers, Signals& signals, Memory& memory)
				{
					switch (signals.ExecutionCycle++)
					{
						case 0:
						{
							registers.BaseAddress = 0;
							signals.AddressBus = registers.PC++;
							memory.Read();
							registers.BaseAddressLow = signals.DataBus;

							return true;
						}
						case 1:
						{
							signals.AddressBus = registers.BaseAddress;
							memory.Read();
							registers.BaseAddressLow += registers.X;

							return true;
						}
						case 2:
						{
							signals.AddressBus = registers.BaseAddress;
							memory.Read();

							std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
							std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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


		constexpr auto OpF9_SBCabsY = [](Registers& registers, Signals& signals, Memory& memory)
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

						registers.InternalRegister = registers.BaseAddressLow + registers.Y;
						registers.BaseAddressLow += registers.Y;

						if (registers.InternalRegister & 0x100) overrun = true;

						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						if (overrun)
						{
							return true;
						}

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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
					case 3:
					{
						registers.BaseAddressHigh++;
						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
						std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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

			constexpr auto OpFD_SBCabsX = [](Registers& registers, Signals& signals, Memory& memory)
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

							registers.InternalRegister = registers.BaseAddressLow + registers.Y;
							registers.BaseAddressLow += registers.Y;

							if (registers.InternalRegister & 0x100) overrun = true;

							signals.AddressBus = registers.BaseAddress;
							memory.Read();

							if (overrun)
							{
								return true;
							}

							std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
							std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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
						case 3:
						{
							registers.BaseAddressHigh++;
							signals.AddressBus = registers.BaseAddress;
							memory.Read();

							std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
							std::uint16_t result = static_cast<std::uint8_t>(registers.A) + static_cast<std::uint8_t>(~signals.DataBus) + static_cast<std::uint8_t>(carry);

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

	}

}
