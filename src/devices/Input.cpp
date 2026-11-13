#include "devices/Input.h"

#include <print>


namespace nvic
{


	auto Input::Initialize() -> void
	{
		for (auto i = 0; i < 512; i++)
			m_Keys[i] = true;
	}


	auto Input::SetKey(std::uint16_t key) -> void
	{
		m_Keys.at(key) = false;

		std::println("Key {} pressed", key);
	}

	auto Input::ReleaseKey(std::uint16_t key) -> void
	{
		m_Keys.at(key) = true;
	}

}
