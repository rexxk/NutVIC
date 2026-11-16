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

		if (id == 0)
		{
			m_Memory.Data[0xC02] = 0x3F;
			m_Memory.Data[0xC03] = 0x7F;
		}
	}


	auto CIA::Update() -> void
	{
		if (m_ID == 0)
		{
			auto& interruptControl = m_Memory.Data[0xC00 + CIA_InterruptControl];

			if (m_Signals.CIA_ReadDC0D)
			{
				m_Memory.Data[0xC00 + CIA_InterruptControl] = 0x00;
				m_Signals.CIA_ReadDC0D = false;
			}


			if (m_Signals.Datassette_READ)
			{
				m_Memory.Data[0xC00 + CIA_InterruptControl] |= 0x10;
				m_Signals.Datassette_READ = false;
			}

			// Timer A
			{
				m_TimerA_Low = m_Registers.CIA1_TimerA_LowByte_Latch;
				m_TimerA_High = m_Registers.CIA1_TimerA_HighByte_Latch;

				m_Memory.Data[0xC00 + CIA_TimerALowByte] = m_TimerA_Value & 0xFF;
				m_Memory.Data[0xC00 + CIA_TimerAHighByte] = m_TimerA_Value >> 8;

				auto& timerAControlFlag = m_Memory.Data[0xC00 + CIA_ControlTimerA];

				if (timerAControlFlag & 0x10)
				{
					m_TimerA_Value = m_TimerA_Latch;
					timerAControlFlag &= ~0x10;
				}

				if (timerAControlFlag & 0x01)
				{
					if ((m_TimerA_Value--) == 0xFFFF)
					{
						m_Memory.Data[0xC00 + CIA_InterruptControl] |= 0x01;

						if (timerAControlFlag & 0x08)
						{
							timerAControlFlag &= ~0x01;
						}
						else
						{
							m_TimerA_Value = m_TimerA_Latch;
						}
					}
				}

			}

			// Timer B
			{
				m_TimerB_Low = m_Registers.CIA1_TimerB_LowByte_Latch;
				m_TimerB_High = m_Registers.CIA1_TimerB_HighByte_Latch;

				m_Memory.Data[0xC00 + CIA_TimerBLowByte] = m_TimerB_Value & 0xFF;
				m_Memory.Data[0xC00 + CIA_TimerBHighByte] = m_TimerB_Value >> 8;

				auto& timerBControlFlag = m_Memory.Data[0xC00 + CIA_ControlTimerB];

				if (timerBControlFlag & 0x10)
				{
					m_TimerB_Value = m_TimerB_Latch;
					timerBControlFlag &= ~0x10;
				}

				if (timerBControlFlag & 0x01)
				{
					if ((m_TimerB_Value--) == 0xFFFF)
					{
						m_Memory.Data[0xC00 + CIA_InterruptControl] |= 0x02;

						if (timerBControlFlag & 0x08)
						{
							timerBControlFlag &= ~0x01;
						}
						else
						{
							m_TimerB_Value = m_TimerB_Latch;
						}
					}
				}

			}

//			auto& interruptMask = m_Registers.CIA1_InterruptMask;
//			interruptControl &= m_Registers.CIA1_InterruptMask;

			if (interruptControl & m_Registers.CIA1_InterruptMask)
				m_Signals.InterruptRequest = true;

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
