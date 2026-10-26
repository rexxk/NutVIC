#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op69_ADC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint8_t value = signals.DataBus;

				std::uint8_t carry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);

				std::uint16_t result = registers.A + value + carry;

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
				if (~(registers.A ^ value) & (registers.A ^ static_cast<std::uint8_t>(result)) & 0x80)
					registers.Flags |= Flag_Overflow;
				else
					registers.Flags &= ~Flag_Overflow;

				return 2;
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

				return 2;
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

				return 2;
			};

		constexpr auto OpE6_INCzp = [](Registers& registers, Signals& signals, Memory& memory)
			{
				signals.AddressBus = registers.PC++;
				memory.Read();
				std::uint16_t zeropageAddress = signals.DataBus;

				signals.AddressBus = zeropageAddress;
				memory.Read();
				std::uint8_t value = signals.DataBus;

				value++;

				signals.DataBus = value;
				memory.Write();

				if (value == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				if (value & 0x80)
					registers.Flags |= Flag_Negative;
				else
					registers.Flags &= ~Flag_Negative;

				return 5;
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

				return 2;
			};

	}

}
