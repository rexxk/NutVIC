#pragma once

#include <cstdint>


namespace nvic
{

	constexpr std::uint8_t Flag_Carry = 0x01;
	constexpr std::uint8_t Flag_Zero = 0x02;
	constexpr std::uint8_t Flag_Interrupt = 0x04;
	constexpr std::uint8_t Flag_Decimal = 0x08;
	constexpr std::uint8_t Flag_Break = 0x10;
	constexpr std::uint8_t Flag_Unused = 0x20;
	constexpr std::uint8_t Flag_Overflow = 0x40;
	constexpr std::uint8_t Flag_Negative = 0x80;


}
