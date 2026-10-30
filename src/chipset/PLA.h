#pragma once

#include "cpu/Signals.h"

#include <atomic>
#include <thread>


namespace nvic
{

	class PLA
	{
	public:
		PLA(Signals& signals);

		auto Start() -> void;
		auto Stop() -> void;

	private:
		auto ThreadFunc() -> void;

	private:
		Signals& m_Signals;

		std::jthread m_Thread;

		std::atomic<bool> m_Running{ false };
	};


}
