#pragma once

#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op20_JSR = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.InternalRegister = signals.DataBus;

						return true;
					}
					case 1:
					{
						std::uint8_t highAddress = registers.PC >> 8;
						memory.Push(0x100 + registers.StackPointer--, highAddress);

						return true;
					}
					case 2:
					{
						std::uint8_t lowAddress = registers.PC & 0xFF;
						memory.Push(0x100 + registers.StackPointer--, lowAddress);

						return true;
					}
					case 3:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.InternalRegister += signals.DataBus << 8;

						return true;
					}
					case 4:
					{
						registers.PC = registers.InternalRegister;
						signals.Executing = false;
					}
				}

				return true;
			};

		constexpr auto Op40_RTI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						memory.Read();
						return true;
					}

					case 1:
					{
						memory.Read();
						registers.StackPointer++;

						return true;
					}

					case 2:
					{
						signals.AddressBus = 0x100 + registers.StackPointer++;
						memory.Read();
						registers.InternalRegister8 = signals.DataBus;

						registers.Flags = registers.InternalRegister8 & 0xCF;

						return true;
					}

					case 3:
					{
						signals.AddressBus = 0x100 + registers.StackPointer++;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 4:
					{
						signals.AddressBus = 0x100 + registers.StackPointer++;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						registers.PC = registers.BaseAddress;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op4C_JMP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.InternalRegister = signals.DataBus;
						return true;
					}
					case 1:
					{
						signals.AddressBus = registers.PC;
						memory.Read();
						registers.InternalRegister += (signals.DataBus << 8);
						registers.PC = registers.InternalRegister;

						signals.Executing = false;

						return true;
					}
				}

				return false;
			};

		constexpr auto Op60_RTS = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						memory.Read();

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + ++registers.StackPointer;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 2:
					{
						signals.AddressBus = 0x100 + ++registers.StackPointer;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						registers.PC = (registers.PC & 0xFF00) + registers.BaseAddressLow;

						return true;
					}

					case 3:
					{
						memory.Read();

						registers.PC = (registers.BaseAddressHigh << 8) + (registers.PC & 0xFF);

						return true;
					}

					case 4:
					{
						registers.PC++;
						signals.Executing = false;
						return true;
					}

				}

				return true;
			};

		constexpr auto Op6C_JMPind = [](Registers& registers, Signals& signals, Memory& memory)
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

						registers.InternalRegister = registers.BaseAddress;

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.InternalRegister;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.InternalRegister + 1;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						registers.PC = registers.BaseAddress;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpFF_IRQ = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC;
						memory.Read();
						registers.StackPointer--;

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + registers.StackPointer--;
						signals.DataBus = registers.PC >> 8;
						memory.Write();

						return true;
					}

					case 2:
					{
						signals.AddressBus = 0x100 + registers.StackPointer--;
						signals.DataBus = registers.PC & 0xFF;
						memory.Write();

						return true;
					}

					case 3:
					{
						signals.AddressBus = 0x100 + registers.StackPointer--;
						registers.InternalRegister8 = registers.Flags;
						signals.DataBus = registers.InternalRegister8;
						memory.Write();

						registers.Flags |= Flag_Interrupt | Flag_Unused;

						return true;
					}

					case 4:
					{
						signals.AddressBus = 0xFFFE;
						memory.Read();
						registers.BaseAddressLow = signals.DataBus;

						return true;
					}

					case 5:
					{
						signals.AddressBus = 0xFFFF;
						memory.Read();
						registers.BaseAddressHigh = signals.DataBus;

						registers.PC = registers.BaseAddress;

						signals.Executing = false;
						return true;
					}
				}

				signals.Executing = false;
				return true;
			};

	}

}
