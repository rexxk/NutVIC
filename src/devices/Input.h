#pragma once

#include <array>
#include <cstdint>


namespace nvic
{

	class Input
	{
	public:
		static auto Initialize() -> void;

		static auto SetKey(std::uint16_t key) -> void;
		static auto ReleaseKey(std::uint16_t key) -> void;

		static auto Keys() -> const std::array<bool, 512>& { return m_Keys; }

	private:
		inline static std::array<bool, 512> m_Keys;
	};

}
