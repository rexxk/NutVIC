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
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (!(registers.Flags & Flag_Negative))
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

		constexpr auto Op30_BMI = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (registers.Flags & Flag_Negative)
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

		constexpr auto Op90_BCC = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (!(registers.Flags & Flag_Carry))
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

		constexpr auto OpB0_BCS = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (registers.Flags & Flag_Carry)
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

		constexpr auto OpD0_BNE = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (!(registers.Flags & Flag_Zero))
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

		constexpr auto OpF0_BEQ = [](Registers& registers, Signals& signals, Memory& memory)
			{
				std::int8_t relativeOffset{};

				std::uint8_t page = registers.PC >> 8;

				signals.AddressBus = registers.PC++;
				memory.Read();
				relativeOffset = static_cast<std::int8_t>(signals.DataBus);

				if (registers.Flags & Flag_Zero)
				{
					registers.PC += relativeOffset;

					std::uint8_t newPage = registers.PC >> 8;

					// Page boundary cross
					if (page != newPage)
						return 4;

					return 3;
				}

				return 2;
			};

	}

}
