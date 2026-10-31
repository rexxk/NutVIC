#pragma once

#include "cpu/Flags.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	namespace opcode
	{

		constexpr auto Op84_STYzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.Y;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op85_STAzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op86_STXzp = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.X;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op8C_STYabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.Y;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op8D_STAabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op8E_STXabs = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.DataBus = registers.X;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op91_STAindY = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.AddressBus = registers.BaseAddress;
						memory.Read();

						std::uint16_t result = static_cast<std::uint16_t>(registers.BaseAddressLow) + registers.Y;

						registers.BaseAddressLow = result & 0xFF;

						if (result & 0x100)
							registers.BaseAddressHigh++;

//						if (static_cast<std::uint16_t>(registers.BaseAddressLow += registers.Y) & 0x100)
//							registers.BaseAddressHigh++;

						return true;
					}

					case 4:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;			
			};

		constexpr auto Op94_STYzpX = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.AddressBus = (registers.BaseAddressLow + registers.X) & 0xFF;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.Y;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op95_STAzpX = [](Registers& registers, Signals& signals, Memory& memory)
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
						signals.AddressBus = (registers.BaseAddressLow + registers.X) & 0xFF;
						memory.Read();

						return true;
					}

					case 2:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op99_STAabsY = [](Registers& registers, Signals& signals, Memory& memory)
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
						std::uint16_t result = static_cast<std::uint16_t>(registers.BaseAddressLow) + registers.Y;

						registers.BaseAddressLow = result & 0xFF;

						if (result & 0x100)
							registers.BaseAddressHigh++;

//						if (static_cast<std::uint16_t>(registers.BaseAddressLow += registers.Y) & 0x100)
//							registers.BaseAddressHigh++;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

		constexpr auto Op9D_STAabsX = [](Registers& registers, Signals& signals, Memory& memory)
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
						std::uint16_t result = static_cast<std::uint16_t>(registers.BaseAddressLow) + registers.X;

						registers.BaseAddressLow = result & 0xFF;

						if (result & 0x100)
							registers.BaseAddressHigh++;

//						if (static_cast<std::uint16_t>(registers.BaseAddressLow += registers.X) & 0x100)
//							registers.BaseAddressHigh++;

						return true;
					}

					case 3:
					{
						signals.AddressBus = registers.BaseAddress;
						signals.DataBus = registers.A;
						memory.Write();

						signals.Executing = false;
						return true;
					}
				}

				return true;
			};

	}

}
