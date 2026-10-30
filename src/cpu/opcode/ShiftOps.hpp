#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op0A_ASL = [](Registers& registers, Signals& signals, Memory& memory)
			{
				if (registers.A & 0x80)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				registers.A <<= 1;

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

		constexpr auto Op2A_ROL = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::uint8_t oldCarry = static_cast<std::uint8_t>(registers.Flags & Flag_Carry);
				
				if (registers.A & 0x80)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				registers.A <<= 1;
				registers.A += oldCarry;

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

		constexpr auto Op46_LSRzp = [](Registers& registers, Signals& signals, Memory& memory)
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
					signals.AddressBus = registers.BaseAddress;
					memory.Write();

					if (registers.InternalRegister8 & 0x01)
						registers.Flags |= Flag_Carry;
					else
						registers.Flags &= ~Flag_Carry;

					registers.InternalRegister8 >>= 1;

					return true;
				}

				case 3:
				{
					signals.AddressBus = registers.BaseAddress;
					signals.DataBus = registers.InternalRegister8;
					memory.Write();

					if (registers.A == 0)
						registers.Flags |= Flag_Zero;
					else
						registers.Flags &= ~Flag_Zero;

					registers.Flags &= 0x7F;

					signals.Executing = false;
					return true;
				}
				}

				return true;
			};

		constexpr auto Op4A_LSR = [](Registers& registers, Signals& signals, Memory& memory)
			{
				if (registers.A & 0x01)
					registers.Flags |= Flag_Carry;
				else
					registers.Flags &= ~Flag_Carry;

				registers.A >>= 1;

				if (registers.A == 0)
					registers.Flags |= Flag_Zero;
				else
					registers.Flags &= ~Flag_Zero;

				registers.Flags &= 0x7F;

				signals.Executing = false;
				return true;
			};

	}

}
