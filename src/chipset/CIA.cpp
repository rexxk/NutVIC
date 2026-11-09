#include "chipset/CIA.h"
#include "cpu/Signals.h"
#include "cpu/Registers.h"

#include <chrono>
#include <print>


namespace nvic
{

	// 50/60Hz clock taken from grid frequency via 9V connector
	constexpr std::uint8_t TODClockNTSC = 60;
	constexpr std::uint8_t TODClockPAL = 50;

	std::chrono::steady_clock::time_point CIA1_TimerA_StartTime{};
	std::uint16_t fps{ 0 };


	CIA::CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals, Registers& registers)
		: m_ID(id), m_Memory(ioMemory), m_Signals(signals), m_Registers(registers)
	{
		m_TODClock = TODClockNTSC;

//		CIA1_TimerA_StartTime = std::chrono::steady_clock::now();
	}


	auto CIA::Update() -> void
	{
		if (m_ID == 0)
		{
			m_TimerA_Low = m_Registers.CIA1_TimerA_LowByte_Latch;
			m_TimerA_High = m_Registers.CIA1_TimerA_HighByte_Latch;

			m_TimerB_Low = m_Registers.CIA1_TimerB_LowByte_Latch;
			m_TimerB_High = m_Registers.CIA1_TimerB_HighByte_Latch;

			if (m_Memory.Data[0xC00 + CIA_InterruptControl] & 0x1)
			{
				if (!m_Running)
				{
//					m_TimerA_Low = m_Memory.Data[0xC04];
//					m_TimerA_High = m_Memory.Data[0xC05];

					m_TimerA_Latch = m_TimerA_Value;

					CIA1_TimerA_StartTime = std::chrono::steady_clock::now();
					
					m_Running = true;
				}

				if ((m_TimerA_Value--) == 0xFFFF)
				{
					m_TimerA_Value = m_TimerA_Latch;
					m_Signals.InterruptRequest = true;
					fps++;
				}

				m_Memory.Data[0xC00 + CIA_TimerALowByte] = m_TimerA_Value & 0xFF;
				m_Memory.Data[0xC00 + CIA_TimerAHighByte] = m_TimerA_Value >> 8;

				auto endTime = std::chrono::steady_clock::now();

				if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - CIA1_TimerA_StartTime).count() / 1000.0 > 1.0)
				{
					CIA1_TimerA_StartTime = endTime;
//					std::println("Latch: {}, FPS: {}", m_TimerA_Latch, fps);
					fps = 0;
				}
			}
			else
				m_Running = false;
		}

		if (m_ID == 1)
		{
			m_Memory.Data[0xD00] |= 0x80;

			m_TimerA_Low = m_Registers.CIA2_TimerA_LowByte_Latch;
			m_TimerA_High = m_Registers.CIA2_TimerA_HighByte_Latch;

			m_TimerB_Low = m_Registers.CIA2_TimerB_LowByte_Latch;
			m_TimerB_High = m_Registers.CIA2_TimerB_HighByte_Latch;
		}
	}


}
