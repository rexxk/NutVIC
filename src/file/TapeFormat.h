#pragma once

#include <cstdint>
#include <vector>


namespace nvic
{


	struct TAPFile
	{
		std::uint8_t ID[12];
		std::uint8_t Version;
		std::uint8_t Reserved[3];

		std::uint32_t DataSize;

		std::vector<std::uint8_t> Data;
	};



}
