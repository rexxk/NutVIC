#pragma once

#include <cstdint>


namespace nvic
{

	struct Registers
	{
		// CPU registers
		std::uint8_t A{};
		std::uint8_t X{};
		std::uint8_t Y{};
		std::uint8_t StackPointer{};
		std::uint8_t Flags{};
		std::uint16_t PC{};

		// CPU internals
		std::uint16_t InternalRegister{};
		std::uint8_t InternalRegister8{};

		union
		{
			struct
			{
				std::uint8_t BaseAddressLow;
				std::uint8_t BaseAddressHigh;
			};

			std::uint16_t BaseAddress;
		};

		std::int8_t RelativeOffset;
	};

}
