#include "CPU/CPU.h"

#include "cpu/opcode/BranchOps.hpp"
#include "cpu/opcode/CompareOps.hpp"
#include "cpu/opcode/ExecutionControl.hpp"
#include "cpu/opcode/LoadOps.hpp"
#include "cpu/opcode/MathOps.hpp"
#include "cpu/opcode/StackOps.hpp"
#include "cpu/opcode/StatusControl.hpp"

#include "memory/Memory.h"

#include <chrono>
#include <cstdint>
#include <print>


namespace nvic
{

	constexpr std::uint16_t ResetVector = 0xFFFC;

	constexpr auto NotImplementedOpCode = [](Registers& registers, Signals& signals, Memory& memory)
		{
			std::println("OpCode {:02x} is not implemented", signals.DataBus);

			return 1;
		};


	CPU::CPU(Clock& clock)
		: m_Clock(clock)
	{
		m_Registers.PC = ResetVector;

		for (auto i = 0; i < 256; i++)
			m_OpCodes.at(i) = NotImplementedOpCode;

		m_OpCodes[0x20] = opcode::Op20_JSR;
		m_OpCodes[0x4C] = opcode::Op4C_JMP;
		m_OpCodes[0x60] = opcode::Op60_RTS;
		m_OpCodes[0x6C] = opcode::Op6C_JMPind;
		m_OpCodes[0x78] = opcode::Op78_SEI;
		m_OpCodes[0x9A] = opcode::Op9A_TXS;
		m_OpCodes[0xA2] = opcode::OpA2_LDX;
		m_OpCodes[0xBD] = opcode::OpBD_LDAabsX;
		m_OpCodes[0xCA] = opcode::OpCA_DEX;
		m_OpCodes[0xD0] = opcode::OpD0_BNE;
		m_OpCodes[0xD8] = opcode::OpD8_CLD;
		m_OpCodes[0xDD] = opcode::OpDD_CMPabsX;
	}

	auto CPU::SetMemory(Memory* memory) -> void
	{
		m_Memory = memory;
	}

	auto CPU::ExecuteThread() -> void
	{
		std::uint64_t startCycle{};
		std::uint64_t cycleCount{};
		std::uint64_t elapsedCycles{};

		std::uint64_t hostStartCycle{};
		std::uint64_t hostExecutionStartCycle{};

		while (m_Running)
		{
			hostStartCycle = __rdtsc();
			startCycle = m_Clock.CPUClockCycles();

			if (m_RunMode == RunMode::Step)
			{
				if (!m_SingleShot)
					continue;

				m_SingleShot.store(false);

//				std::println("Single shot");
			}

			hostExecutionStartCycle = __rdtsc();

			// Execute
			FetchOperand(m_Registers.PC == ResetVector);

			auto cycles = m_OpCodes[m_Operand](m_Registers, m_Signals, *m_Memory);

			m_DecoderHostExecutionTime = __rdtsc() - hostExecutionStartCycle;

			cycleCount = m_Clock.CPUClockCycles() - startCycle;


			if (m_RunMode == RunMode::Step)
			{
				m_OpCodeExecutionTime = cycleCount;
				m_OpCodeCycles = cycles;
			}

			// Delay, wait until cycleCount equals operation cycle time
			do
			{
				m_DecoderCycleTime = m_Clock.CPUClockCycles() - startCycle;
				m_DecoderHostCycleTime = __rdtsc() - hostStartCycle;
			} while (m_DecoderHostCycleTime < cycles * m_Clock.CPUClockDelay());

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

	auto CPU::FetchOperand(bool resetVector) -> void
	{
		if (resetVector)
		{
			m_Operand = 0x4C;	// JMP
			return;
		}

		m_Signals.AddressBus = m_Registers.PC++;
		m_Memory->Read();

		m_Operand = m_Signals.DataBus;
	}
}
