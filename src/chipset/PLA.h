#pragma once

#include "cpu/Signals.h"

#include <atomic>
#include <cstdint>
#include <thread>


namespace nvic
{

	class PLA
	{
	public:
		PLA(Signals& signals);

//		auto Update() -> void;

		auto GetBankSelector(bool read) const -> const std::uint8_t;

	private:
		Signals& m_Signals;

		std::uint8_t m_ActiveCPUPortSignals{ 0 };
	};


}
