#pragma once

#include <cstdint>


namespace nvic
{


	class PIC
	{
	public:
		PIC(std::uint8_t id);

	private:
		std::uint8_t m_ID{};
	};


}
