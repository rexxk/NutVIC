#pragma once

#include "cpu/Signals.h"

#include <array>
#include <cstdint>


namespace nvic
{

	constexpr std::uint16_t ChunkSize = 16 * 1024;

	struct MemoryChunk
	{
		bool ReadWrite{ true };
		std::array<std::uint8_t, ChunkSize> Data;
	};

	class Memory
	{
	public:
		Memory(Signals& signals);

		auto Read() -> void;
		auto Write() -> void;

		auto SetChunk(std::uint8_t chunkID, MemoryChunk& chunk) -> void;
		auto FindChunk(std::uint16_t address) -> std::uint8_t;

		auto ViewMemory() -> void;

		auto GetChunks() -> std::array<MemoryChunk*, 4>& { return m_Chunks; }

	private:
		Signals& m_Signals;

		std::array<MemoryChunk*, 4> m_Chunks;


		int m_PageNumber{ 0xFF };
	};

}
