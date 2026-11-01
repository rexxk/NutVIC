#include "chipset/CIA.h"

#include <chrono>
#include <print>


namespace nvic
{

	// 50/60Hz clock taken from grid frequency via 9V connector
	constexpr std::uint8_t TODClockNTSC = 60;
	constexpr std::uint8_t TODClockPAL = 50;

//	std::chrono::steady_clock::time_point CIA1_TimerA_StartTime{};


	CIA::CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals)
		: m_ID(id), m_Memory(ioMemory), m_Signals(signals)
	{
		m_TODClock = TODClockNTSC;

//		CIA1_TimerA_StartTime = std::chrono::steady_clock::now();
	}


	auto CIA::Update() -> void
	{
		if (m_ID == 0)
		{
			if (m_Memory.Data[0xC00 + CIA_InterruptControl] & 0x1)
			{
				if (!m_Running)
				{
					m_TimerA_Low = m_Memory.Data[0xC04];
					m_TimerA_High = m_Memory.Data[0xC05];

					m_TimerA_Value = m_TimerA_Latch;

					m_Running = true;
				}

				if ((m_TimerA_Value--) == 0xFFFF)
				{
					m_TimerA_Value = m_TimerA_Latch;
					m_Signals.InterruptRequest = true;
				}

				m_Memory.Data[0xC00 + CIA_TimerALowByte] = m_TimerA_Value & 0xFF;
				m_Memory.Data[0xC00 + CIA_TimerAHighByte] = m_TimerA_Value >> 8;
			}
			else
				m_Running = false;
		}

		if (m_ID == 1)
		{
			m_Memory.Data[0xD00] |= 0x80;
		}
	}


}
