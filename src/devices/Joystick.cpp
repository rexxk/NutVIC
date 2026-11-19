#include "devices/Joystick.h"
#include "devices/Input.h"

#include "memory/Memory.h"

#include <cstdint>

#include <GLFW/glfw3.h>


namespace nvic
{

	Joystick::Joystick(MemoryChunk& ioMemory, std::uint8_t port)
		: m_IOMemory(ioMemory), m_Port(port)
	{

	}


	auto Joystick::Update() -> void
	{
		auto& keys = Input::Keys();

		std::uint8_t value{ 0xFF };

		if (!keys[GLFW_KEY_KP_8]) value &= ~0x01;
		if (!keys[GLFW_KEY_KP_2]) value &= ~0x02;
		if (!keys[GLFW_KEY_KP_4]) value &= ~0x04;
		if (!keys[GLFW_KEY_KP_6]) value &= ~0x08;
		if (!keys[GLFW_KEY_KP_0]) value &= ~0x10;

		m_IOMemory.Data[0xC00 + 1 - m_Port] &= value;
	}

}
