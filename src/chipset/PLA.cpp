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
		m_ActiveCPUPortSignals = 0x00;

	}

	auto PLA::GetBankSelector(bool read) const -> const std::uint8_t
	{
		auto bank = m_Signals.AddressBus >> 12;

		if (read)
		{
			if (m_Signals.BusAvailable)
			{
				if (bank == 0xA || bank == 0xB || bank == 0xE || bank == 0xF)
					return 1;
				else if (bank == 0xD)
					return 2;
				else
					return 0;
			}
			else
			{
				if (bank == 0xA || bank == 0xB || bank == 0xE || bank == 0xF)
					return 1;
				else
					return 0;
			}
		}
		else
		{
			if (bank == 0xD)
			{
				m_Signals.WriteToIO = true;
				return 2;
			}
			else
				return 0;
		}

		return 0;
	}

//	auto PLA::Update() -> void
//	{
//		m_Signals.BusAvailable
//		m_Signals.Read
//
//
//
//		if (m_Signals.CPU_IOPort != m_ActiveCPUPortSignals)
//		{
//			m_ActiveCPUPortSignals = m_Signals.CPU_IOPort;
//
//			// true - BASIC ROM
//			// false - RAM
//			if (m_Signals.CPU_IOPort & CPUIO_LORAM)
//			{
//				m_Memory.SetBank(0xA, 1);
//				m_Memory.SetBank(0xB, 1);
//			}
//			else
//			{
//				m_Memory.SetBank(0xA, 0);
//				m_Memory.SetBank(0xB, 0);
//			}
//
//			// true = KERNAL ROM
//			// false = RAM
//			if (m_Signals.CPU_IOPort & CPUIO_HIRAM)
//			{
//				m_Memory.SetBank(0xE, 1);
//				m_Memory.SetBank(0xF, 1);
//			}
//			else
//			{
//				m_Memory.SetBank(0xE, 0);
//				m_Memory.SetBank(0xF, 0);
//			}
//
//			// true = IO
//			// false = CHARGEN ROM
//			if (m_Signals.CPU_IOPort & CPUIO_CHAREN)
//			{
//				m_Memory.SetBank(0xD, 1);
//			}
//			else
//			{
//				m_Memory.SetBank(0xD, 0);
//			}
//
//
//		}
//	}

}
