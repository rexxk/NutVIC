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

		auto Read(std::uint64_t cpuClockTicks) -> void;

		auto GetFilePointer() const -> const std::uint32_t { return m_FilePointer; }

	private:
		Signals& m_Signals;

		std::filesystem::path m_Filepath{};

		std::uint32_t m_FilePointer{};
		std::uint64_t m_StartCycleTick{};
		std::uint64_t m_WaitCycles{};

		TAPFile m_Image;

		bool m_CassetteInserted{ false };

		friend class DatassettePanel;
	};


}
