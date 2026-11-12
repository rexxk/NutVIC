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

		m_Memory.Data[0xC00] = 0xFF;
		m_Memory.Data[0xC01] = 0xFF;

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
			m_Memory.Data[0xC00] &= ~0x02;
			m_Memory.Data[0xC01] &= ~0x04;
		}
	}

}
