#pragma once

#include "chipset/Clock.h"
#include "cpu/Registers.h"
#include "cpu/Signals.h"

#include <array>
#include <atomic>
#include <functional>
#include <thread>


namespace nvic
{

	using OpCodeFn = std::function<void()>;

	class Memory;

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

		auto SetMemory(Memory* memory) -> void;
		auto SetRunMode(RunMode mode) -> void;

		auto GetRegisters() -> Registers& { return m_Registers; }
		auto GetSignals() -> Signals& { return m_Signals; }

		auto Start() -> void;
		auto Stop() -> void;

		auto SingleShot() -> void;

	private:
		auto ExecuteThread() -> void;

		auto FetchOperand(bool resetVector) -> void;

	private:
		Clock& m_Clock;
		Memory* m_Memory{ nullptr };

		std::atomic<RunMode> m_RunMode{ RunMode::Step };

		Registers m_Registers{};
		Signals m_Signals{};

		std::jthread m_Thread;
		std::atomic<bool> m_Running{ false };
		std::atomic<bool> m_SingleShot{ false };


		std::array<OpCodeFn, 256> m_OpCodes;
		std::uint8_t m_Operand{};
	};

}
