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

		m_ImageName = filepath.stem().string();

		fp.close();
	}

	auto Datassette::Eject() -> void
	{

	}

	auto Datassette::Read(std::uint64_t cpuClockTicks) -> void
	{
		auto elapsedTicks = cpuClockTicks - m_StartCycleTick;

		std::uint32_t waitTime = m_Image.Data[m_FilePointer] * 8;

		if (waitTime == 0)
		{
			if (m_Image.Version == 0) waitTime = 256;
			else if (m_Image.Version == 1)
			{
				m_FilePointer++;

				for (auto i = 0; i < 3; i++)
				{
					waitTime += m_Image.Data[m_FilePointer++] << (8 * i);

					if (m_FilePointer == m_Image.DataSize)
					{
						m_FilePointer = m_Image.DataSize - 1;
						m_Signals.Datassette_PLAY = false;
					}
				}
			}
		}

		if (elapsedTicks >= waitTime) // && !m_Signals.Datassette_READ)
		{
//			if (elapsedTicks > waitTime)
//			{
//				std::println("Overflow @{}: {} ({})", m_FilePointer, elapsedTicks, waitTime);
//			}

			m_Signals.Datassette_READ = true;
			m_StartCycleTick = cpuClockTicks;

//			std::println("Cycles: {}", elapsedTicks);

			m_FilePointer++;
			if (m_FilePointer == m_Image.DataSize)
			{
				m_FilePointer = m_Image.DataSize - 1;
				m_Signals.Datassette_PLAY = false;
			}
		}

	}


}
