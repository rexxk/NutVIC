#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <atomic>
#include <cstdint>
#include <thread>


namespace nvic
{

	class PLA
	{
	public:
		PLA(Signals& signals, Memory& memory);

		auto Start() -> void;
		auto Stop() -> void;

	private:
		auto ThreadFunc() -> void;

	private:
		Signals& m_Signals;
		Memory& m_Memory;

		std::jthread m_Thread;

		std::atomic<bool> m_Running{ false };

		std::uint8_t m_ActiveCPUPortSignals{ 0 };
	};


}
