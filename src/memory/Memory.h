#pragma once

#include "cpu/Signals.h"

#include <array>
#include <cstdint>


namespace nvic
{

	constexpr std::uint16_t ChunkSize = 4 * 1024;

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

		auto Push(std::uint16_t address, std::uint8_t value) -> void;
		auto Pop(std::uint16_t address) -> const std::uint8_t;

		auto SetChunk(std::uint8_t chunkID, MemoryChunk& chunk) -> void;
		auto FindChunk(std::uint16_t address) -> std::uint8_t;

		auto ViewMemory() -> void;

		auto GetChunks() -> std::array<MemoryChunk*, 16>& { return m_Chunks; }

	private:
		Signals& m_Signals;

		std::array<MemoryChunk*, 16> m_Chunks;


		int m_PageNumber{ 0xFF };
	};

}
