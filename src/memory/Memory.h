#pragma once

#include "cpu/Signals.h"

#include <array>
#include <cstdint>
#include <vector>


namespace nvic
{

	constexpr std::uint16_t ChunkSize = 4 * 1024;

	class MemoryPanel;

	struct MemoryChunk
	{
		bool ReadWrite{ true };
		std::array<std::uint8_t, ChunkSize> Data;
	};

	class Memory
	{
	public:
		Memory(Signals& signals);

		auto SetDefaultValues() -> void;

		auto Read() -> void;
		auto Write() -> void;

		auto ReadAddress(std::uint16_t address) -> std::uint8_t;

		auto Push(std::uint16_t address, std::uint8_t value) -> void;
		auto Pop(std::uint16_t address) -> const std::uint8_t;

		auto SetChunk(std::uint8_t chunkID, std::uint8_t select, MemoryChunk& chunk) -> void;
		auto SetChunk(std::uint8_t chunkID, std::uint8_t select, std::vector<MemoryChunk>& chunks) -> void;
		auto FindChunk(std::uint16_t address) -> std::uint8_t;

		auto GetChunks() -> std::array<MemoryChunk*[2], 16>& { return m_Chunks; }

		auto SetBank(std::uint8_t bank, std::uint8_t select) -> void;

	private:
		Signals& m_Signals;

		std::array<MemoryChunk*[2], 16> m_Chunks;

		std::array<std::uint8_t, 16> m_BankSelector{};

		int m_PageNumber{ 0xFF };

		friend class MemoryPanel;
	};

}
