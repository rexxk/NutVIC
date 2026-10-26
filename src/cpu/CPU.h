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

	class Memory;

	using OpCodeFn = std::function<std::uint8_t(Registers&, Signals&, Memory&)>;

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

		auto Reset() -> void;

		auto SetMemory(Memory* memory) -> void;
		auto SetRunMode(RunMode mode) -> void;
		auto GetRunMode() const -> const RunMode { return m_RunMode; }

		auto GetRegisters() -> Registers& { return m_Registers; }
		auto GetSignals() -> Signals& { return m_Signals; }

		auto Start() -> void;
		auto Stop() -> void;

		auto SingleShot() -> void;

		auto GetLastExecutionTime() const -> const std::uint64_t { return m_OpCodeExecutionTime; }
		auto GetLastExecutionCycles() const -> const std::uint8_t { return m_OpCodeCycles; }
		auto GetDecoderCycleTime() const -> const std::uint64_t { return m_DecoderCycleTime; }
		auto GetDecoderHostCycleTime() const -> const std::uint64_t { return m_DecoderHostCycleTime; }
		auto GetDecoderHostExecutionTime() const -> const std::uint64_t { return m_DecoderHostExecutionTime; }
		auto GetOpsRun() const -> const std::uint64_t { return m_OpsRun; }

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

		std::uint64_t m_OpCodeExecutionTime{};
		std::uint8_t m_OpCodeCycles{};
		std::uint64_t m_DecoderHostExecutionTime{};
		std::uint64_t m_DecoderCycleTime{};
		std::uint64_t m_DecoderHostCycleTime{};
		std::uint64_t m_OpsRun{};

		std::array<OpCodeFn, 256> m_OpCodes;
		std::uint8_t m_Operand{};
	};

}
