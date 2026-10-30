#include "chipset/VIC_II.h"

#include "chipset/Clock.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"
#include <chrono>
#include <print>

using namespace std::chrono_literals;


constexpr std::uint16_t VIC_MemoryBankRegister = 0xD00;

constexpr std::uint8_t VIC_MemoryBanks = 0x03;


namespace nvic
{

	VIC_II::VIC_II(Clock& clock, MemoryChunk& ioMemory, std::array<MemoryChunk, 16>& ram, Signals& signals)
		: m_Clock(clock), m_IOMemory(ioMemory), m_RAM(ram), m_Signals(signals)
	{
		// Set default values
		m_IOMemory.Data[VIC_ControlRegister1] = 0x9B;
		m_IOMemory.Data[VIC_ControlRegister2] = 0x08;
		m_IOMemory.Data[VIC_MemoryPointers] = 0x14;
		m_IOMemory.Data[VIC_InterruptRegister] = 0x0F;
	}


	auto VIC_II::Start() -> void
	{
		m_Running.store(true);
		m_Thread = std::jthread(&VIC_II::ThreadFunc, this);
	}

	auto VIC_II::Stop() -> void
	{
		m_Running.store(false);
		m_Thread.join();
	}

	auto VIC_II::ThreadFunc() -> void
	{
//		std::this_thread::sleep_for(3s);

//		std::println("VIC-II is hijacking address enable control signal");

		std::uint64_t cycles{};
		std::uint16_t rasterLine{};

		while (m_Running)
		{
			cycles = m_Clock.CPUClockCycles();

			if (m_Signals.Control & !Signal_ClockPhase1 && !m_Signals.BusAvailable)
			{
//				m_Signals.BusLock = true;







//				m_Signals.BusLock = false;

				m_Signals.BusAvailable = true;
			}

			if ((cycles % 63) == 0)
				rasterLine++;

			if (rasterLine == 312)
				rasterLine = 0;

			m_IOMemory.Data[VIC_RasterCounter] = static_cast<std::uint8_t>(rasterLine & 0xFF);

			if (rasterLine & 0x100)
				m_IOMemory.Data[VIC_ControlRegister1] |= (1 << 7);
			else
				m_IOMemory.Data[VIC_ControlRegister1] &= ~(1 << 7);

//			m_Signals.Control &= ~Signal_AddressEnableControl;

		}

	}

	auto VIC_II::Update() -> void
	{
		// Invert select
		m_RAMSelect = 3 - m_IOMemory.Data[VIC_MemoryBankRegister] & VIC_MemoryBanks;

		m_BaseAddress = m_IOMemory.Data[VIC_MemoryPointers];
	}

}
