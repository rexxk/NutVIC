#pragma once

#include <cstdint>

#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{


	constexpr std::uint8_t CIA_DataPortA = 0x00;
	constexpr std::uint8_t CIA_DataPortB = 0x01;
	constexpr std::uint8_t CIA_DataDirectionPortA = 0x02;
	constexpr std::uint8_t CIA_DataDirectionPortB = 0x03;
	constexpr std::uint8_t CIA_TimerALowByte = 0x04;
	constexpr std::uint8_t CIA_TimerAHighByte = 0x05;
	constexpr std::uint8_t CIA_TimerBLowByte = 0x06;
	constexpr std::uint8_t CIA_TimerBHighByte = 0x07;
	constexpr std::uint8_t CIA_RTCTenths = 0x08;
	constexpr std::uint8_t CIA_RTCSeconds = 0x09;
	constexpr std::uint8_t CIA_RTCMinutes = 0x0A;
	constexpr std::uint8_t CIA_RTCHours = 0x0B;
	constexpr std::uint8_t CIA_SerialShiftRegister = 0x0C;
	constexpr std::uint8_t CIA_InterruptControl = 0x0D;
	constexpr std::uint8_t CIA_ControlTimerA = 0x0E;
	constexpr std::uint8_t CIA_ControlTimerB = 0x0F;



	class PIC
	{
	public:
		PIC(std::uint8_t id, MemoryChunk& memoryChunk, Signals& signals);

	private:
		MemoryChunk& m_IOMemory;
		Signals& m_Signals;

		std::uint8_t m_ID{};
		std::uint16_t m_BaseAddress{};

	};


}
