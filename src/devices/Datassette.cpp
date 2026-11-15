#include "devices/Datassette.h"

#include <fstream>
#include <print>


namespace nvic
{

	Datassette::Datassette(Signals& signals)
		: m_Signals(signals)
	{

	}

	auto Datassette::OpenTapeImage(const std::filesystem::path& filepath) -> void
	{
		std::ifstream fp(filepath, std::ios::in | std::ios::binary);


		if (!fp.is_open())
		{
			std::println("Unable to open .tap file {}", filepath.string());
			return;
		}

		fp.seekg(0, fp.end);
		std::size_t filesize = fp.tellg();
		fp.seekg(0, fp.beg);

		fp.read((char*)&m_Image, 20);

		if (std::strncmp((char*)&m_Image.ID, "C64-TAPE-RAW", 12))
		{
			std::println("Not a valid tap file, header mismatch!");
			fp.close();

			return;
		}

		m_Image.Data.resize(m_Image.DataSize);
		fp.read((char*)m_Image.Data.data(), m_Image.DataSize);

		std::println("Opening image: {} ({}) bytes, data: {} bytes", filepath.stem().string(), filesize, m_Image.DataSize);

		fp.close();
	}

	auto Datassette::Eject() -> void
	{

	}

}
