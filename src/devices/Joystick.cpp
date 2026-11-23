#include "devices/Joystick.h"
#include "devices/Input.h"

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <cstdint>
#include <print>

#include <GLFW/glfw3.h>


namespace nvic
{

	Joystick::Joystick(MemoryChunk& ioMemory, Signals& signals, std::uint8_t port)
		: m_IOMemory(ioMemory), m_Signals(signals), m_Port(port)
	{

	}


	auto Joystick::Update() -> void
	{
		auto& keys = Input::Keys();

		std::uint8_t value{ 0xFF };

		std::uint8_t direction = m_IOMemory.Data[0xC02 + m_Port];

		if (direction != 0xFF)
		{
			if (!keys[GLFW_KEY_KP_8]) value &= ~0x01;
			if (!keys[GLFW_KEY_KP_2]) value &= ~0x02;
			if (!keys[GLFW_KEY_KP_4]) value &= ~0x04;
			if (!keys[GLFW_KEY_KP_6]) value &= ~0x08;
			if (!keys[GLFW_KEY_KP_0]) value &= ~0x10;

			//		if ((m_IOMemory.Data[0xC00 + 1 - m_Port] == 0x7F) || (m_IOMemory.Data[0xC00 + 1 - m_Port] == 0xFF))
			//		if (!m_Signals.CIA_WriteDC00)
			m_IOMemory.Data[0xC00 + m_Port] &= value;
		}
	}

}
