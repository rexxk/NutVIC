#include "chipset/VIC_II.h"

#include "cpu/Signals.h"
#include "memory/Memory.h"
#include <chrono>
#include <print>

using namespace std::chrono_literals;


namespace nvic
{

	VIC_II::VIC_II(MemoryChunk& memory, Signals& signals)
		: m_Memory(memory), m_Signals(signals)
	{

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
		std::this_thread::sleep_for(3s);

		m_Signals.Control &= ~Signal_AddressEnableControl;

		std::println("VIC-II is hijacking address enable control signal");

		while (m_Running)
		{

//			m_Memory.Data[0x12] = 0;

		}

	}

}
