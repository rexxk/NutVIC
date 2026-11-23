#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <array>
#include <cstdint>


namespace nvic
{


	class Keyboard
	{
	public:
		Keyboard(MemoryChunk& ioMemory, Signals& signals);

		auto Update() -> void;

	private:
		MemoryChunk& m_Memory;
		Signals& m_Signals;

		std::array<std::uint8_t, 8> m_KeyMatrix{};
	};


}
