#include "CPU/CPU.h"

#include <cstdint>
#include <print>


namespace nvic
{

	constexpr std::uint16_t ResetVector = 0xFFFC;


	CPU::CPU(Clock& clock)
		: m_Clock(clock)
	{
		m_Registers.PC = ResetVector;


	}

	auto CPU::ExecuteThread() -> void
	{
		std::uint64_t cycleCount{};

		while (m_Running)
		{
			if (m_RunMode == RunMode::Step)
			{
				if (!m_SingleShot)
					continue;

				m_SingleShot.store(false);

				std::println("Single shot");
			}

			auto startCycle = m_Clock.CPUClockCycles();

			// Execute



			cycleCount = m_Clock.CPUClockCycles() - startCycle;

			// Delay, wait until cycleCount equals operation cycle time
		}
	}

	auto CPU::Start() -> void
	{
		m_Running.store(true);
		m_Thread = std::jthread(&CPU::ExecuteThread, this);
	}

	auto CPU::Stop() -> void
	{
		m_Running.store(false);
		m_Thread.join();
	}

	auto CPU::SingleShot() -> void
	{
		m_SingleShot.store(true);
	}

	auto CPU::SetRunMode(RunMode mode) -> void
	{
		m_RunMode.store(mode);
	}

}
