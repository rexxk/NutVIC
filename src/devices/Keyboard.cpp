#include "devices/Keyboard.h"
#include "devices/Input.h"

#include <GLFW/glfw3.h>

#include <print>


namespace nvic
{

	Keyboard::Keyboard(MemoryChunk& ioMemory)
		: m_Memory(ioMemory)
	{
		// Inverse logic in the matrix

		for (auto i = 0; i < 8; i++)
			m_KeyMatrix[i] = 0xFF;
	}

	auto Keyboard::Update() -> void
	{
		auto& keys = Input::Keys();

		auto rowSelect = m_Memory.Data[0xC00];

		std::uint8_t col = 0xFF;

		switch (rowSelect)
		{
			case 0xFE:
			{
				if (!keys[GLFW_KEY_DELETE]) col &= ~0x01;
				if (!keys[GLFW_KEY_ENTER]) col &= ~0x02;
				if (!keys[GLFW_KEY_RIGHT]) col &= ~0x04;
				if (!keys[GLFW_KEY_F7]) col &= ~0x08;
				if (!keys[GLFW_KEY_F1]) col &= ~0x10;
				if (!keys[GLFW_KEY_F3]) col &= ~0x20;
				if (!keys[GLFW_KEY_F5]) col &= ~0x40;
				if (!keys[GLFW_KEY_DOWN]) col &= ~0x80;

				break;
			}
			case 0xFD:
			{
				if (!keys[GLFW_KEY_3]) col &= ~0x01;
				if (!keys[GLFW_KEY_W]) col &= ~0x02;
				if (!keys[GLFW_KEY_A]) col &= ~0x04;
				if (!keys[GLFW_KEY_4]) col &= ~0x08;
				if (!keys[GLFW_KEY_Z]) col &= ~0x10;
				if (!keys[GLFW_KEY_S]) col &= ~0x20;
				if (!keys[GLFW_KEY_E]) col &= ~0x40;
				if (!keys[GLFW_KEY_LEFT_SHIFT]) col &= ~0x80;

				break;
			}
			case 0xFB:
			{
				if (!keys[GLFW_KEY_5]) col &= ~0x01;
				if (!keys[GLFW_KEY_R]) col &= ~0x02;
				if (!keys[GLFW_KEY_D]) col &= ~0x04;
				if (!keys[GLFW_KEY_6]) col &= ~0x08;
				if (!keys[GLFW_KEY_C]) col &= ~0x10;
				if (!keys[GLFW_KEY_F]) col &= ~0x20;
				if (!keys[GLFW_KEY_T]) col &= ~0x40;
				if (!keys[GLFW_KEY_X]) col &= ~0x80;

				break;
			}
			case 0xF7:
			{
				if (!keys[GLFW_KEY_7]) col &= ~0x01;
				if (!keys[GLFW_KEY_Y]) col &= ~0x02;
				if (!keys[GLFW_KEY_G]) col &= ~0x04;
				if (!keys[GLFW_KEY_8]) col &= ~0x08;
				if (!keys[GLFW_KEY_B]) col &= ~0x10;
				if (!keys[GLFW_KEY_H]) col &= ~0x20;
				if (!keys[GLFW_KEY_U]) col &= ~0x40;
				if (!keys[GLFW_KEY_V]) col &= ~0x80;

				break;
			}
			case 0xEF:
			{
				if (!keys[GLFW_KEY_9]) col &= ~0x01;
				if (!keys[GLFW_KEY_I]) col &= ~0x02;
				if (!keys[GLFW_KEY_J]) col &= ~0x04;
				if (!keys[GLFW_KEY_0]) col &= ~0x08;
				if (!keys[GLFW_KEY_M]) col &= ~0x10;
				if (!keys[GLFW_KEY_K]) col &= ~0x20;
				if (!keys[GLFW_KEY_O]) col &= ~0x40;
				if (!keys[GLFW_KEY_N]) col &= ~0x80;

				break;
			}
			case 0xDF:
			{
				if (!keys[GLFW_KEY_KP_ADD]) col &= ~0x01;
				if (!keys[GLFW_KEY_P]) col &= ~0x02;
				if (!keys[GLFW_KEY_L]) col &= ~0x04;
				if (!keys[GLFW_KEY_EQUAL]) col &= ~0x08;
				if (!keys[GLFW_KEY_PERIOD]) col &= ~0x10;
				if (!keys[GLFW_KEY_SEMICOLON]) col &= ~0x20;
				if (!keys[GLFW_KEY_LEFT_BRACKET]) col &= ~0x40;
				if (!keys[GLFW_KEY_COMMA]) col &= ~0x80;

				break;
			}
			case 0xBF:
			{
				if (!keys[GLFW_KEY_WORLD_2]) col &= ~0x01;
				if (!keys[GLFW_KEY_RIGHT_BRACKET]) col &= ~0x02;
				if (!keys[GLFW_KEY_APOSTROPHE]) col &= ~0x04;
				if (!keys[GLFW_KEY_HOME]) col &= ~0x08;
				if (!keys[GLFW_KEY_RIGHT_SHIFT]) col &= ~0x10;
				if (!keys[GLFW_KEY_BACKSLASH]) col &= ~0x20;
				if (!keys[GLFW_KEY_END]) col &= ~0x40;
				if (!keys[GLFW_KEY_SLASH]) col &= ~0x80;

				break;
			}
			case 0x7F:
			{
				if (!keys[GLFW_KEY_1]) col &= ~0x01;
				if (!keys[GLFW_KEY_GRAVE_ACCENT]) col &= ~0x02;
				if (!keys[GLFW_KEY_TAB]) col &= ~0x04;
				if (!keys[GLFW_KEY_2]) col &= ~0x08;
				if (!keys[GLFW_KEY_SPACE]) col &= ~0x10;
				if (!keys[GLFW_KEY_LEFT_CONTROL]) col &= ~0x20;
				if (!keys[GLFW_KEY_Q]) col &= ~0x40;
				if (!keys[GLFW_KEY_ESCAPE]) col &= ~0x80;

				break;
			}
		}

		m_Memory.Data[0xC01] = col;
	}

}
