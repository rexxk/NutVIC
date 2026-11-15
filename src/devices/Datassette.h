#pragma once

#include "cpu/Signals.h"
#include "file/TapeFormat.h"

#include <cstdint>
#include <filesystem>

namespace nvic
{


	class DatassettePanel;

	class Datassette
	{
	public:
		Datassette(Signals& signals);

		auto OpenTapeImage(const std::filesystem::path& filepath) -> void;
		auto Eject() -> void;

	private:
		Signals& m_Signals;

		std::filesystem::path m_Filepath{};


		TAPFile m_Image;

		friend class DatassettePanel;
	};


}
