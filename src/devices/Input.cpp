#include "devices/Input.h"

#include <print>


namespace nvic
{


	auto Input::Initialize() -> void
	{
		for (auto i = 0; i < 512; i++)
			m_Keys[i] = true;

		m_Keys[296] = false;
	}


	auto Input::SetKey(std::uint8_t key) -> void
	{
		m_Keys[key] = false;

		std::println("Key {} pressed", key);
	}

	auto Input::ReleaseKey(std::uint8_t key) -> void
	{
		m_Keys[key] = true;
	}

}
