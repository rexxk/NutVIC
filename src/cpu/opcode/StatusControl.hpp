#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op08_PHP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = 0x100 + registers.StackPointer;
						memory.Read();

						registers.InternalRegister8 = registers.Flags;
						registers.InternalRegister8 |= Flag_Break | Flag_Unused;

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + registers.StackPointer--;
						signals.DataBus = registers.InternalRegister8;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op18_CLC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Carry;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op28_PLP = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + registers.StackPointer++;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = 0x100 + registers.StackPointer;
						memory.Read();

						registers.Flags = signals.DataBus & 0xCF;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op38_SEC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags |= Flag_Carry;

				signals.Executing = false;
				return true;
			};


		constexpr auto Op48_PHA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + registers.StackPointer--;
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op58_CLI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Interrupt;

				signals.Executing = false;
				return true;
			};

		constexpr auto Op68_PLA = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.StackPointer++;

						return true;
					}

					case 1:
					{
						signals.AddressBus = 0x100 + registers.StackPointer;
						memory.Read();

						registers.A = signals.DataBus;

						return true;
					}

					case 2:
					{
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

		constexpr auto Op78_SEI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags |= Flag_Interrupt;

				signals.Executing = false;
				return true;
			};

		constexpr auto OpD8_CLD = [](Registers& registers, Signals& signals, Memory& memory)
			{
				registers.Flags &= ~Flag_Decimal;

				signals.Executing = false;
				return true;
			};
	}

}
