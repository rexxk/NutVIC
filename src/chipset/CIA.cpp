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


	CIA::CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals, Registers& registers)
		: m_ID(id), m_Memory(ioMemory), m_Signals(signals), m_Registers(registers)
	{
		m_TODClock = TODClockNTSC;

//		if (id == 0)
//		{
//			m_Memory.Data[0xC02] = 0x3F;
//			m_Memory.Data[0xC03] = 0x7F;
//		}

		m_TimerA_Latch = 0xFFFF;
		m_TimerB_Latch = 0xFFFF;
	}


	auto CIA::Update() -> void
	{
		std::uint16_t startAddress = 0xC00 + m_ID * 0x100;

		if (m_ID == 0)
		{
			m_TimerA_Low = m_Registers.CIA1_TimerA_LowByte_Latch;
			m_TimerA_High = m_Registers.CIA1_TimerA_HighByte_Latch;

			if (m_Signals.CIA_WriteDC05 && !(m_Memory.Data[0xC00 + CIA_ControlTimerA] & 0x01))
			{
				m_TimerA_Value = m_TimerA_Latch;
			}

			m_TimerB_Low = m_Registers.CIA1_TimerB_LowByte_Latch;
			m_TimerB_High = m_Registers.CIA1_TimerB_HighByte_Latch;

			if (m_Signals.CIA_WriteDC07 && !(m_Memory.Data[0xC00 + CIA_ControlTimerB] & 0x01))
			{
				m_TimerB_Value = m_TimerB_Latch;
			}

			m_Signals.CIA_WriteDC05 = false;
			m_Signals.CIA_WriteDC07 = false;

			if (m_Signals.CIA_ReadDC0D)
			{
				m_Memory.Data[0xC00 + CIA_InterruptControl] = 0x00;
				m_Signals.CIA_ReadDC0D = false;
			}
		}
		else
		{
			m_TimerA_Low = m_Registers.CIA2_TimerA_LowByte_Latch;
			m_TimerA_High = m_Registers.CIA2_TimerA_HighByte_Latch;

			m_TimerB_Low = m_Registers.CIA2_TimerB_LowByte_Latch;
			m_TimerB_High = m_Registers.CIA2_TimerB_HighByte_Latch;

			if (m_Signals.CIA_ReadDD0D)
			{
				m_Memory.Data[0xD00 + CIA_InterruptControl] = 0x00;
				m_Signals.CIA_ReadDD0D = false;
			}
		}

		auto& interruptControl = m_Memory.Data[startAddress + CIA_InterruptControl];


		if (m_Signals.Datassette_READ)
		{
			if (m_Registers.CIA1_InterruptMask & 0x10)
			{
				interruptControl |= 0x90;
				m_Signals.Datassette_READ = false;
			}
		}

		// Timer A
		{
			m_Memory.Data[startAddress + CIA_TimerALowByte] = m_TimerA_Value & 0xFF;
			m_Memory.Data[startAddress + CIA_TimerAHighByte] = m_TimerA_Value >> 8;

			auto& timerAControlFlag = m_Memory.Data[startAddress + CIA_ControlTimerA];

			if (timerAControlFlag & 0x10)
			{
				m_TimerA_Value = m_TimerA_Latch;
				timerAControlFlag &= ~0x10;
			}

			if (timerAControlFlag & 0x01)
			{
				m_TimerA_Value--;

				if (m_TimerA_Value == 0xFFFF)
				{
					m_TimerA_Value = m_TimerA_Latch;

					if (m_Registers.CIA1_InterruptMask & 0x01)
					{
						interruptControl |= 0x81;
					}

					if (timerAControlFlag & 0x08)
					{
						timerAControlFlag &= ~0x01;
					}
				}
			}

		}

		// Timer B
		{
			m_Memory.Data[startAddress + CIA_TimerBLowByte] = m_TimerB_Value & 0xFF;
			m_Memory.Data[startAddress + CIA_TimerBHighByte] = m_TimerB_Value >> 8;

			auto& timerBControlFlag = m_Memory.Data[startAddress + CIA_ControlTimerB];

			if (timerBControlFlag & 0x10)
			{
				m_TimerB_Value = m_TimerB_Latch;
				timerBControlFlag &= ~0x10;
			}

			if (timerBControlFlag & 0x01)
			{
				m_TimerB_Value--;

				if (m_TimerB_Value == 0xFFFF)
				{
					m_TimerB_Value = m_TimerB_Latch;

					if (m_Registers.CIA1_InterruptMask & 0x02)
					{
						interruptControl |= 0x82;
					}

					if (timerBControlFlag & 0x08)
					{
						timerBControlFlag &= ~0x01;
					}
				}
			}
		}

		m_PendingInterrupt = (m_Memory.Data[0xC00 + CIA_InterruptControl] & 0x80) | (m_Memory.Data[0xD00 + CIA_InterruptControl] & 0x80);
	}



}
