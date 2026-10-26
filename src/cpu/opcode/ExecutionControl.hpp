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
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint8_t destinationLowAddress = signals.DataBus;

				std::uint8_t highAddress = registers.PC >> 8;
				memory.Push(0x100 + registers.StackPointer--, highAddress);

				std::uint8_t lowAddress = registers.PC & 0xFF;
				memory.Push(0x100 + registers.StackPointer--, lowAddress);

				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint8_t destinationHighAddress = signals.DataBus;

				std::uint16_t newAddress = static_cast<std::uint16_t>(destinationHighAddress << 8) + destinationLowAddress;
				registers.PC = newAddress;

				return 6;
			};

		constexpr auto Op4C_JMP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint16_t address = signals.DataBus;

				signals.AddressBus = registers.PC;
				memory.Read();
				address += (signals.DataBus << 8);

				registers.PC = address;

				return 3;
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
