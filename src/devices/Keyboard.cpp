#include "devices/Keyboard.h"
#include "devices/Input.h"

#include <GLFW/glfw3.h>

#include <print>


namespace nvic
{

	bool xkey{ true };

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

		std::uint8_t col1 = 0xFF;
		std::uint8_t col2 = 0xFF;
		std::uint8_t col3 = 0xFF;
		std::uint8_t col4 = 0xFF;
		std::uint8_t col5 = 0xFF;
		std::uint8_t col6 = 0xFF;
		std::uint8_t col7 = 0xFF;
		std::uint8_t col8 = 0xFF;

		if (!keys[GLFW_KEY_DELETE]) col1 &= ~0x01;
		if (!keys[GLFW_KEY_ENTER]) col1 &= ~0x02;
		if (!keys[GLFW_KEY_RIGHT]) col1 &= ~0x04;
		if (!keys[GLFW_KEY_F7]) col1 &= ~0x08;
		if (!keys[GLFW_KEY_F1]) col1 &= ~0x10;
		if (!keys[GLFW_KEY_F3]) col1 &= ~0x20;
		if (!keys[GLFW_KEY_F5]) col1 &= ~0x40;
		if (!keys[GLFW_KEY_DOWN]) col1 &= ~0x80;

		if (!keys[GLFW_KEY_3]) col2 &= ~0x01;
		if (!keys[GLFW_KEY_W]) col2 &= ~0x02;
		if (!keys[GLFW_KEY_A]) col2 &= ~0x04;
//		col2 &= ~0x04;
		if (!keys[GLFW_KEY_4]) col2 &= ~0x08;
		if (!keys[GLFW_KEY_Z]) col2 &= ~0x10;
		if (!keys[GLFW_KEY_S]) col2 &= ~0x20;
		if (!keys[GLFW_KEY_E]) col2 &= ~0x40;
		if (!keys[GLFW_KEY_LEFT_SHIFT]) col2 &= ~0x80;

		if (!keys[GLFW_KEY_5]) col3 &= ~0x01;
		if (!keys[GLFW_KEY_R]) col3 &= ~0x02;
		if (!keys[GLFW_KEY_D]) col3 &= ~0x04;
		if (!keys[GLFW_KEY_6]) col3 &= ~0x08;
		if (!keys[GLFW_KEY_C]) col3 &= ~0x10;
		if (!keys[GLFW_KEY_F]) col3 &= ~0x20;
		if (!keys[GLFW_KEY_T]) col3 &= ~0x40;
		if (xkey)
		{
			if (!keys[GLFW_KEY_X]) col3 &= ~0x80;
		}
		else
		{
			col3 &= ~0x80;
		}



		if (!keys[GLFW_KEY_7]) col4 &= ~0x01;
		if (!keys[GLFW_KEY_Y]) col4 &= ~0x02;
		if (!keys[GLFW_KEY_G]) col4 &= ~0x04;
		if (!keys[GLFW_KEY_8]) col4 &= ~0x08;
		if (!keys[GLFW_KEY_B]) col4 &= ~0x10;
		if (!keys[GLFW_KEY_H]) col4 &= ~0x20;
		if (!keys[GLFW_KEY_U]) col4 &= ~0x40;
		if (!keys[GLFW_KEY_V]) col4 &= ~0x80;

		if (!keys[GLFW_KEY_9]) col5 &= ~0x01;
		if (!keys[GLFW_KEY_I]) col5 &= ~0x02;
		if (!keys[GLFW_KEY_J]) col5 &= ~0x04;
		if (!keys[GLFW_KEY_0]) col5 &= ~0x08;
		if (!keys[GLFW_KEY_M]) col5 &= ~0x10;
		if (!keys[GLFW_KEY_K]) col5 &= ~0x20;
		if (!keys[GLFW_KEY_O]) col5 &= ~0x40;
		if (!keys[GLFW_KEY_N]) col5 &= ~0x80;

		if (!keys[GLFW_KEY_KP_ADD]) col6 &= ~0x01;
		if (!keys[GLFW_KEY_P]) col6 &= ~0x02;
		if (!keys[GLFW_KEY_L]) col6 &= ~0x04;
		if (!keys[GLFW_KEY_EQUAL]) col6 &= ~0x08;
		if (!keys[GLFW_KEY_PERIOD]) col6 &= ~0x10;
		if (!keys[GLFW_KEY_SEMICOLON]) col6 &= ~0x20;
		if (!keys[GLFW_KEY_LEFT_BRACKET]) col6 &= ~0x40;
		if (!keys[GLFW_KEY_COMMA]) col6 &= ~0x80;

		if (!keys[GLFW_KEY_WORLD_2]) col7 &= ~0x01;
		if (!keys[GLFW_KEY_RIGHT_BRACKET]) col7 &= ~0x02;
		if (!keys[GLFW_KEY_APOSTROPHE]) col7 &= ~0x04;
		if (!keys[GLFW_KEY_HOME]) col7 &= ~0x08;
		if (!keys[GLFW_KEY_RIGHT_SHIFT]) col7 &= ~0x10;
		if (!keys[GLFW_KEY_BACKSLASH]) col7 &= ~0x20;
		if (!keys[GLFW_KEY_END]) col7 &= ~0x40;
		if (!keys[GLFW_KEY_SLASH]) col7 &= ~0x80;

		if (!keys[GLFW_KEY_1]) col8 &= ~0x01;
		if (!keys[GLFW_KEY_GRAVE_ACCENT]) col8 &= ~0x02;
		if (!keys[GLFW_KEY_TAB]) col8 &= ~0x04;
		if (!keys[GLFW_KEY_2]) col8 &= ~0x08;
		if (!keys[GLFW_KEY_SPACE]) col8 &= ~0x10;
		if (!keys[GLFW_KEY_LEFT_CONTROL]) col8 &= ~0x20;
		if (!keys[GLFW_KEY_Q]) col8 &= ~0x40;
		if (!keys[GLFW_KEY_ESCAPE]) col8 &= ~0x80;

		switch (rowSelect)
		{
			case 0x00:
			{
				col = col1 & col2 & col3 & col4 & col5 & col6 & col7 & col8;
				break;
			}

			case 0xFE:
			{
				col = col1;
				break;
			}
			case 0xFD:
			{
				col = col2;
				break;
			}
			case 0xFB:
			{
				col = col3;
				break;
			}
			case 0xF7:
			{
				col = col4;
				break;
			}
			case 0xEF:
			{
				col = col5;
				break;
			}
			case 0xDF:
			{
				col = col6;
				break;
			}
			case 0xBF:
			{
				col = col7;
				break;
			}
			case 0x7F:
			{
				col = col8;
				break;
			}

			case 0xFF:
			{
				col = 0xFF;
				break;
			}

			case 0xBD:
			{
				col = col2 & col7;
				break;
			}

			default:
			{
				std::println("Invalid row: {}", rowSelect);
				col = 0xFF;
				break;
			}
		}

		m_Memory.Data[0xC01] = col;
	}

	auto Keyboard::SetXButton() -> void
	{
		xkey = !xkey;
	}

}
