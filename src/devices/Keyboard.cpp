#include "devices/Keyboard.h"
#include "memory/Memory.h"

#include <print>


namespace nvic
{


	Keyboard::Keyboard(MemoryChunk& ioMemory)
		: m_Memory(ioMemory)
	{
		for (auto i = 0; i < 256; i++)
			m_Keys[i] = false;
	}


	auto Keyboard::SetKey(std::uint8_t key) -> void
	{
		m_Keys[key] = true;
	}

	auto Keyboard::ReleaseKey(std::uint8_t key) -> void
	{
		m_Keys[key] = false;
	}

	auto Keyboard::UpdateMatrix(const std::uint8_t key) -> void
	{
		if (key == 'A')
		{
			m_Memory.Data[0] |= 0x02;
			m_Memory.Data[1] |= 0x04;
		}
	}

}
