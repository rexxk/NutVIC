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


	PLA::PLA(Signals& signals)
		: m_Signals(signals)
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
			auto address = m_Signals.AddressBus;

			if (m_Signals.CPU_IOPort & CPUIO_LORAM)
				// true - BASIC ROM
				// false - RAM
				;

			if (m_Signals.CPU_IOPort & CPUIO_HIRAM)
				// true = KERNAL ROM
				// false = RAM
				;

			if (m_Signals.CPU_IOPort & CPUIO_CHAREN)
				// true = IO
				// false = CHARGEN ROM
				;


		}
	}

}
