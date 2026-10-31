#include "chipset/PLA.h"

#include "cpu/Signals.h"

#include <atomic>
#include <thread>


namespace nvic
{

	constexpr std::uint8_t CPUIO_LORAM = 0x01;
	constexpr std::uint8_t CPUIO_HIRAM = 0x02;
	constexpr std::uint8_t CPUIO_CHAREN = 0x04;
	constexpr std::uint8_t CPUIO_CassetteWrite = 0x08;
	constexpr std::uint8_t CPUIO_CassetteSwitchSense = 0x10;
	constexpr std::uint8_t CPUIO_CassetteMotorControl = 0x20;


	PLA::PLA(Signals& signals, Memory& memory)
		: m_Signals(signals), m_Memory(memory)
	{

	}

	auto PLA::Start() -> void
	{
		m_Thread = std::jthread(&PLA::ThreadFunc, this);
		m_Running.store(true);
	}

	auto PLA::Stop() -> void
	{
		m_Running.store(false);
		m_Thread.join();
	}

	auto PLA::ThreadFunc() -> void
	{
		while (m_Running.load())
		{
			if (m_Signals.CPU_IOPort != m_ActiveCPUPortSignals)
			{
				m_ActiveCPUPortSignals = m_Signals.CPU_IOPort;

				// true - BASIC ROM
				// false - RAM
				if (m_Signals.CPU_IOPort & CPUIO_LORAM)
				{
					m_Memory.SetBank(0xA, 1);
					m_Memory.SetBank(0xB, 1);
				}
				else
				{
					m_Memory.SetBank(0xA, 0);
					m_Memory.SetBank(0xB, 0);
				}

				// true = KERNAL ROM
				// false = RAM
				if (m_Signals.CPU_IOPort & CPUIO_HIRAM)
				{
					m_Memory.SetBank(0xE, 1);
					m_Memory.SetBank(0xF, 1);
				}
				else
				{
					m_Memory.SetBank(0xE, 0);
					m_Memory.SetBank(0xF, 0);
				}

				// true = IO
				// false = CHARGEN ROM
				if (m_Signals.CPU_IOPort & CPUIO_CHAREN)
				{
					m_Memory.SetBank(0xD, 1);
				}
				else
				{
					m_Memory.SetBank(0xD, 0);
				}

			}



		}
	}

}
