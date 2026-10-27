#pragma once

#include <cstdint>


namespace nvic
{

	constexpr std::uint16_t Signal_InterruptRequest = 0x0001;
	constexpr std::uint16_t Signal_Ready = 0x0002;
	constexpr std::uint16_t Signal_NMIRequest = 0x0004;
	constexpr std::uint16_t Signal_Reset = 0x0008;
	constexpr std::uint16_t Signal_ReadWrite = 0x0010;
	constexpr std::uint16_t Signal_AddressEnableControl = 0x0020;
	constexpr std::uint16_t Signal_LowRam = 0x0100;
	constexpr std::uint16_t Signal_HighRam = 0x0200;
	constexpr std::uint16_t Signal_CharEnable = 0x0400;
	constexpr std::uint16_t Signal_CassetteWrite = 0x0800;
	constexpr std::uint16_t Signal_CassetteSense = 0x1000;
	constexpr std::uint16_t Signal_CassetteMotor = 0x2000;
	constexpr std::uint16_t Signal_ClockPhase1 = 0x8000;

	struct Signals
	{
		std::uint16_t Control{};

		bool Executing{};
		std::uint8_t ExecutionCycle{};

		std::uint8_t DataBus{};
		std::uint16_t AddressBus{};
	};

}
