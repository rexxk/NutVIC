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
				std::uint8_t lowAddress = memory.Pop(0x100 + ++registers.StackPointer);
				std::uint8_t highAddress = memory.Pop(0x100 + ++registers.StackPointer);

				std::uint16_t newAddress = static_cast<std::uint16_t>(highAddress << 8) + lowAddress;

				registers.PC = newAddress + 1;

				return 6;
			};

		constexpr auto Op6C_JMPind = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint16_t address{};
				std::uint16_t jmpAddress{};

				signals.AddressBus = registers.PC++;
				memory.Read();
				address = signals.DataBus;

				signals.AddressBus = registers.PC++;
				memory.Read();
				address += signals.DataBus << 8;

				signals.AddressBus = address++;
				memory.Read();
				jmpAddress = signals.DataBus;

				signals.AddressBus = address++;
				memory.Read();
				jmpAddress += signals.DataBus << 8;

				registers.PC = jmpAddress;

				return 5;
			};

	}

}
