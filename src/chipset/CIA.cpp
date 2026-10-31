#include "chipset/CIA.h"

#include <chrono>
#include <print>


namespace nvic
{

	// 50/60Hz clock taken from grid frequency via 9V connector
	constexpr std::uint8_t TODClockNTSC = 60;
	constexpr std::uint8_t TODClockPAL = 50;

	std::chrono::steady_clock::time_point CIA1_TimerA_StartTime{};

	std::uint16_t TimerA_Value{};


	CIA::CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals)
		: m_ID(id), m_Memory(ioMemory), m_Signals(signals)
	{
		m_TODClock = TODClockNTSC;

		CIA1_TimerA_StartTime = std::chrono::steady_clock::now();
	}


	auto CIA::Update() -> void
	{
		if (m_ID == 0)
		{
			if (m_Memory.Data[0xC00 + CIA_InterruptControl] & 0x1)
			{
				auto TimerA_EndTime = std::chrono::steady_clock::now();

				if ((std::chrono::duration_cast<std::chrono::milliseconds>(TimerA_EndTime - CIA1_TimerA_StartTime).count() / 1000.0) > (1 / 60.0f))
				{
					m_Signals.InterruptRequest = true;
					CIA1_TimerA_StartTime = TimerA_EndTime;

					TimerA_Value++;
					m_Memory.Data[0xC00 + CIA_TimerALowByte] = TimerA_Value & 0xFF;
					m_Memory.Data[0xC00 + CIA_TimerAHighByte] = TimerA_Value >> 8;
				}
			}
		}

		if (m_ID == 1)
		{
			m_Memory.Data[0xD00] |= 0x80;
		}
	}


}
