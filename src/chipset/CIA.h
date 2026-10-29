#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <cstdint>


namespace nvic
{

	class CIA
	{
	public:
		CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals);

		auto Update() -> void;

	private:
		MemoryChunk& m_Memory;
		Signals& m_Signals;

		std::uint8_t m_ID{};

	};

}
