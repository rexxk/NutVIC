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

		auto Update() -> void;

	private:
		Signals& m_Signals;
		Memory& m_Memory;

		std::uint8_t m_ActiveCPUPortSignals{ 0 };
	};


}
