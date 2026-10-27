#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <atomic>
#include <thread>

namespace nvic
{

	class VIC_II
	{
	public:
		VIC_II(MemoryChunk& memory, Signals& signals);

		auto Start() -> void;
		auto Stop() -> void;

	private:
		void ThreadFunc();

	private:
		MemoryChunk& m_Memory;
		Signals& m_Signals;

		std::atomic<bool> m_Running{ false };

		std::jthread m_Thread;
	};

}
