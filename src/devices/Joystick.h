#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <cstdint>


namespace nvic
{


	class Joystick
	{
	public:
		Joystick(MemoryChunk& ioMemory, Signals& signals, std::uint8_t port);

		auto Update() -> void;

	private:
		MemoryChunk& m_IOMemory;
		Signals& m_Signals;

		std::uint8_t m_Port{};
	};


}
