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

		int JumpToAddress{};
		bool JumpTo{ false };

		bool Executing{};
		std::uint8_t ExecutionCycle{};

		std::uint8_t DataBus{};
		std::uint16_t AddressBus{};

		bool DisableInterrupts{ false };
		bool InterruptRequest{ false };
		bool BusAvailable{ true };

		bool Break{ false };

		std::uint8_t CPU_IOPort{ 0x37 };		

		bool WriteToIO{ false };

		bool PLA_CHARROM{ false };
		bool PLA_BASIC{ false };
		bool PLA_KERNAL{ false };

		bool CIA_ReadDC0D{ false };
		bool CIA_ReadDD0D{ false };
		bool CIA_WriteDC05{ false };
		bool CIA_WriteDC07{ false };

		bool Datassette_SENSE{ true };
		bool Datassette_MOTOR{ true };
		bool Datassette_READ{ false };
		bool Datassette_WRITE{ false };

		bool Datassette_PLAY{ false };
	};

}
