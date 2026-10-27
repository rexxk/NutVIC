#include "CPU/CPU.h"

#include "cpu/opcode/BranchOps.hpp"
#include "cpu/opcode/CompareOps.hpp"
#include "cpu/opcode/ExecutionControl.hpp"
#include "cpu/opcode/LoadOps.hpp"
#include "cpu/opcode/LogicOps.hpp"
#include "cpu/opcode/MathOps.hpp"
#include "cpu/opcode/ShiftOps.hpp"
#include "cpu/opcode/StatusControl.hpp"
#include "cpu/opcode/StoreOps.hpp"
#include "cpu/opcode/TransferOps.hpp"

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

			return 0;
		};


	CPU::CPU(Clock& clock)
		: m_Clock(clock)
	{
		m_Registers.PC = ResetVector;

		for (auto i = 0; i < 256; i++)
			m_OpCodes.at(i) = NotImplementedOpCode;

		m_OpCodes[0x09] = opcode::Op09_ORA;
		m_OpCodes[0x0D] = opcode::Op0D_ORAabs;
		m_OpCodes[0x10] = opcode::Op10_BPL;
		m_OpCodes[0x18] = opcode::Op18_CLC;
		m_OpCodes[0x20] = opcode::Op20_JSR;
		m_OpCodes[0x29] = opcode::Op29_AND;
		m_OpCodes[0x2A] = opcode::Op2A_ROL;
		m_OpCodes[0x30] = opcode::Op30_BMI;
		m_OpCodes[0x4C] = opcode::Op4C_JMP;
		m_OpCodes[0x60] = opcode::Op60_RTS;
		m_OpCodes[0x69] = opcode::Op69_ADC;
		m_OpCodes[0x6C] = opcode::Op6C_JMPind;
		m_OpCodes[0x78] = opcode::Op78_SEI;
		m_OpCodes[0x84] = opcode::Op84_STYzp;
		m_OpCodes[0x85] = opcode::Op85_STAzp;
		m_OpCodes[0x86] = opcode::Op86_STXzp;
		m_OpCodes[0x88] = opcode::Op88_DEY;
		m_OpCodes[0x8A] = opcode::Op8A_TXA;
		m_OpCodes[0x8C] = opcode::Op8C_STYabs;
		m_OpCodes[0x8D] = opcode::Op8D_STAabs;
		m_OpCodes[0x8E] = opcode::Op8E_STXabs;
		m_OpCodes[0x90] = opcode::Op90_BCC;
		m_OpCodes[0x91] = opcode::Op91_STAindY;
		m_OpCodes[0x94] = opcode::Op94_STYzpX;
		m_OpCodes[0x95] = opcode::Op95_STAzpX;
		m_OpCodes[0x98] = opcode::Op98_TYA;
		m_OpCodes[0x99] = opcode::Op99_STAabsY;
		m_OpCodes[0x9A] = opcode::Op9A_TXS;
		m_OpCodes[0x9D] = opcode::Op9D_STAabsX;
		m_OpCodes[0xA0] = opcode::OpA0_LDY;
		m_OpCodes[0xA2] = opcode::OpA2_LDX;
		m_OpCodes[0xA4] = opcode::OpA4_LDYzp;
		m_OpCodes[0xA5] = opcode::OpA5_LDAzp;
		m_OpCodes[0xA6] = opcode::OpA6_LDXzp;
		m_OpCodes[0xA8] = opcode::OpA8_TAY;
		m_OpCodes[0xA9] = opcode::OpA9_LDA;
		m_OpCodes[0xAA] = opcode::OpAA_TAX;
		m_OpCodes[0xAD] = opcode::OpAD_LDAabs;
		m_OpCodes[0xB0] = opcode::OpB0_BCS;
		m_OpCodes[0xB1] = opcode::OpB1_LDAindY;
		m_OpCodes[0xB4] = opcode::OpB4_LDYzpX;
		m_OpCodes[0xB5] = opcode::OpB5_LDAzpX;
		m_OpCodes[0xB6] = opcode::OpB6_LDXzpY;
		m_OpCodes[0xB9] = opcode::OpB9_LDAabsY;
		m_OpCodes[0xBD] = opcode::OpBD_LDAabsX;
		m_OpCodes[0xC1] = opcode::OpC1_CMPidxind;
		m_OpCodes[0xC8] = opcode::OpC8_INY;
		m_OpCodes[0xCA] = opcode::OpCA_DEX;
		m_OpCodes[0xD0] = opcode::OpD0_BNE;
		m_OpCodes[0xD1] = opcode::OpD1_CMPindY;
		m_OpCodes[0xD8] = opcode::OpD8_CLD;
		m_OpCodes[0xDD] = opcode::OpDD_CMPabsX;
		m_OpCodes[0xE0] = opcode::OpE0_CPX;
		m_OpCodes[0xE6] = opcode::OpE6_INCzp;
		m_OpCodes[0xE8] = opcode::OpE8_INX;
		m_OpCodes[0xF0] = opcode::OpF0_BEQ;
	}

	auto CPU::Reset() -> void
	{
		m_Registers.PC = ResetVector;
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

//			if (m_Registers.PC == 0xFD67)
//				m_RunMode = RunMode::Step;

			hostExecutionStartCycle = __rdtsc();

			// Execute
			FetchOperand(m_Registers.PC == ResetVector);

			auto cycles = m_OpCodes[m_Operand](m_Registers, m_Signals, *m_Memory);

			if (cycles == 0)
				m_RunMode = RunMode::Step;

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

			m_OpsRun++;
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
