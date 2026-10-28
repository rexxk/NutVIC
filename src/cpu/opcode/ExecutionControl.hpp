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

	}

}
