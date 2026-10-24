#pragma once

#include "chipset/Clock.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"

#include <atomic>
#include <thread>


namespace nvic
{

	class CPU
	{
	public:
		enum class RunMode
		{
			Step,
			Continuous,
		};

	public:
		CPU(Clock& clock);

		auto SetRunMode(RunMode mode) -> void;

		auto GetRegisters() -> Registers& { return m_Registers; }
		auto GetSignals() -> Signals& { return m_Signals; }

		auto Start() -> void;
		auto Stop() -> void;

		auto SingleShot() -> void;

	private:
		auto ExecuteThread() -> void;

	private:
		Clock& m_Clock;

		std::atomic<RunMode> m_RunMode{ RunMode::Step };

		Registers m_Registers{};
		Signals m_Signals{};

		std::jthread m_Thread;
		std::atomic<bool> m_Running{ false };
		std::atomic<bool> m_SingleShot{ false };
	};

}
