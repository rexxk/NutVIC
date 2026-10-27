#include "chipset/Clock.h"
#include "cpu/Signals.h"

#include <chrono>
#include <print>
#include <string>



namespace nvic
{

	constexpr std::uint64_t CrystalFrequencyNTSC = 14'318'180;
	constexpr std::uint64_t CrystalFrequencyPAL = 17'734'475;

	constexpr std::uint64_t DualVoltageFrequencyNTSC = 8'180'000;
	constexpr std::uint64_t DualVoltageFrequencyPAL = 7'880'000;



	Clock::Clock(FrequencyMode mode, Signals& signals)
		: m_FrequencyMode(mode), m_Signals(signals)
	{
		std::uint64_t currentPerformanceCount{};

		auto performanceCounterStart = _Query_perf_counter();
		auto wait = _Query_perf_frequency();
		auto startCount = __rdtsc();

		do
		{
			currentPerformanceCount = _Query_perf_counter();
		} while ((currentPerformanceCount - performanceCounterStart) < wait);

		m_HostProcessorFrequency = __rdtsc() - startCount;	

		if (m_FrequencyMode == FrequencyMode::PAL)
		{
			m_ColorClockDelay = m_HostProcessorFrequency / CrystalFrequencyPAL;
			m_DotClockDelay = m_HostProcessorFrequency / DualVoltageFrequencyPAL;
			m_CPUClockDelay = m_DotClockDelay * 8;
		}
		else
		{
			m_ColorClockDelay = m_HostProcessorFrequency / CrystalFrequencyNTSC;
			m_DotClockDelay = m_HostProcessorFrequency / DualVoltageFrequencyNTSC;
			m_CPUClockDelay = m_DotClockDelay * 8;
		}

		std::println("Host processor frequency         : {} MHz", static_cast<double>(m_HostProcessorFrequency) / 1'000'000);
		std::println("Host cycles per oscillator cycle : {}", m_ColorClockDelay);
		std::println("Host cycles per clock cycle      : {}", m_DotClockDelay);
		std::println("Host cycles per CPU cycle        : {}", m_CPUClockDelay);
//		std::println("Host cycles per oscillator cycle (NTSC) : {}", m_ProcessorFrequency / CrystalFrequencyNTSC);
//		std::println("Host cycles per clock cycle (NTSC) : {}", m_ProcessorFrequency / DualVoltageFrequencyNTSC);

	}

	auto Clock::Start() -> void
	{
		m_ClockThread = std::jthread(&Clock::ClockThread, this);

		m_Running.store(true);
	}

	auto Clock::Stop() -> void
	{
		m_Running.store(false);

		m_ClockThread.join();
	}

	auto Clock::ClockThread() -> void
	{
		std::string mode{};
		if (m_FrequencyMode == FrequencyMode::PAL) mode = "PAL";
		else if (m_FrequencyMode == FrequencyMode::NTSC) mode = "NTSC";

		std::println("Starting {} clock thread", mode);

		std::uint64_t colorClockStart{};
		std::uint64_t colorClockElapsed{};

		std::uint64_t dotClockStart{};
		std::uint64_t dotClockElapsed{};

		std::uint64_t cpuClockStart{};
		std::uint64_t cpuClockElapsed{};

		cpuClockStart = dotClockStart = colorClockStart = __rdtsc();

		std::uint32_t cpuClockHalfPhaseDelay = m_CPUClockDelay / 2;
		std::uint64_t cpuHalfClockTicks{};

		while (m_Running.load())
		{
			auto now = __rdtsc();

			colorClockElapsed = now - colorClockStart;

			if (colorClockElapsed > m_ColorClockDelay)
			{
				m_ColorClockTicks++;
				colorClockStart = now;
			}

			dotClockElapsed = now - dotClockStart;

			if (dotClockElapsed > m_DotClockDelay)
			{
				m_DotClockTicks++;
				dotClockStart = now;
			}

			cpuClockElapsed = now - cpuClockStart;

//			if (cpuClockElapsed > m_CPUClockDelay)
			if (cpuClockElapsed > cpuClockHalfPhaseDelay)
			{
				cpuHalfClockTicks++;

				if (cpuHalfClockTicks % 2)
				{
					m_Signals.Control |= Signal_AddressEnableControl | Signal_ClockPhase1;
					m_CPUClockTicks++;
					cpuClockStart = now;
				}
				else
				{
					m_Signals.Control &= ~Signal_ClockPhase1;
				}
			}

		}

		std::println("Stopping clock thread");

		std::println("Total ticks:");
		std::println(" - Color: {}", m_ColorClockTicks.load());
		std::println(" - Dot:   {}", m_DotClockTicks.load());
		std::println(" - CPU:   {}", m_CPUClockTicks.load());
	}


}
