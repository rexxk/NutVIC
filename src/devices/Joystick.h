#pragma once

#include "memory/Memory.h"

#include <cstdint>


namespace nvic
{


	class Joystick
	{
	public:
		Joystick(MemoryChunk& ioMemory, std::uint8_t port);

		auto Update() -> void;

	private:
		MemoryChunk& m_IOMemory;

		std::uint8_t m_Port{};
	};


}
