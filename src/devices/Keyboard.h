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

		static auto SetKey(std::uint8_t key) -> void;
		static auto ReleaseKey(std::uint8_t key) -> void;

		auto UpdateMatrix(const std::uint8_t key) -> void;

	private:
		MemoryChunk& m_Memory;

		inline static std::array<bool, 256> m_Keys;
	};

}
