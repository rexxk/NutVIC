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

		// IO latches
		std::uint8_t CIA1_TimerA_LowByte_Latch{};
		std::uint8_t CIA1_TimerA_HighByte_Latch{};
		std::uint8_t CIA1_TimerB_LowByte_Latch{};
		std::uint8_t CIA1_TimerB_HighByte_Latch{};

		std::uint8_t CIA2_TimerA_LowByte_Latch{};
		std::uint8_t CIA2_TimerA_HighByte_Latch{};
		std::uint8_t CIA2_TimerB_LowByte_Latch{};
		std::uint8_t CIA2_TimerB_HighByte_Latch{};


		std::int8_t RelativeOffset;

		std::uint16_t LastPC{};
		std::uint16_t SecondLastPC{};
		std::uint8_t SecondLastOperand{};
		std::uint8_t LastOperand{};
		std::uint8_t ActiveOperand{};
	};

}
