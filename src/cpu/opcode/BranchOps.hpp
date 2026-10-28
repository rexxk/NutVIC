#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op10_BPL = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (registers.Flags & Flag_Negative)
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op30_BMI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (!(registers.Flags & Flag_Negative))
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op90_BCC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (registers.Flags & Flag_Carry)
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpB0_BCS = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (!(registers.Flags & Flag_Carry))
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpD0_BNE = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (registers.Flags & Flag_Zero)
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}
					
						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto OpF0_BEQ = [](Registers& registers, Signals& signals, Memory& memory)
			{
				switch (signals.ExecutionCycle++)
				{
					case 0:
					{
						signals.AddressBus = registers.PC++;
						memory.Read();
						registers.RelativeOffset = static_cast<std::int8_t>(signals.DataBus);

						if (!(registers.Flags & Flag_Zero))
						{
							signals.Executing = false;
							return true;
						}

						return true;
					}

					case 1:
					{
						registers.InternalRegister = registers.PC + registers.RelativeOffset;

						signals.AddressBus = registers.PC;

						if ((registers.InternalRegister & 0xFF00) != (registers.PC & 0xFF00))
						{
							memory.Read();
							return true;
						}

						registers.PC = signals.AddressBus;

						signals.Executing = false;
						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.PC;
						memory.Read();

						registers.PC = registers.InternalRegister;

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};


	}

}
