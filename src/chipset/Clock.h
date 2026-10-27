#pragma once

#include "cpu/Signals.h"

#include <atomic>
#include <cstdint>
#include <thread>


namespace nvic
{

	class Clock
	{
	public:
		enum class FrequencyMode
		{
			PAL,
			NTSC,
		};

	public:
		Clock(FrequencyMode mode, Signals& signals);

		auto Start() -> void;
		auto Stop() -> void;

		auto ColorClockCycles() const -> const std::uint64_t { return m_ColorClockTicks.load(); }
		auto DotClockCycles() const -> const std::uint64_t { return m_DotClockTicks.load(); }
		auto CPUClockCycles() const -> const std::uint64_t { return m_CPUClockTicks.load(); }

		auto ColorClockDelay() const -> const std::uint32_t { return m_ColorClockDelay; }
		auto DotClockDelay() const -> const std::uint32_t { return m_DotClockDelay; }
		auto CPUClockDelay() const -> const std::uint32_t { return m_CPUClockDelay; }

	private:
		auto ClockThread() -> void;

	private:
		Signals& m_Signals;
		FrequencyMode m_FrequencyMode{ FrequencyMode::PAL };

		std::atomic<bool> m_Running{ false };

		std::jthread m_ClockThread{};

		std::uint32_t m_ColorClockDelay{};
		std::uint32_t m_DotClockDelay{};
		std::uint32_t m_CPUClockDelay{};

		std::atomic<std::uint64_t> m_ColorClockTicks{};
		std::atomic<std::uint64_t> m_DotClockTicks{};
		std::atomic<std::uint64_t> m_CPUClockTicks{};

		std::uint64_t m_HostProcessorFrequency{};
	};

}
