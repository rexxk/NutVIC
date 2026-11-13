#pragma once

#include "memory/Memory.h"

#include <array>
#include <cstdint>


namespace nvic
{


	class Keyboard
	{
	public:
		Keyboard(MemoryChunk& ioMemory);

		auto Update() -> void;

	private:
		MemoryChunk& m_Memory;

		std::array<std::uint8_t, 8> m_KeyMatrix{};
	};


}
