#include "chipset/VIC_II.h"

#include "chipset/Clock.h"
#include "cpu/Signals.h"
#include "memory/Memory.h"
#include <chrono>
#include <print>

using namespace std::chrono_literals;


constexpr std::uint16_t VIC_MemoryBankRegister = 0xD00;

constexpr std::uint8_t VIC_MemoryBanks = 0x03;

constexpr std::uint16_t UpperVisibleRasterLineNTSC = 12;
constexpr std::uint16_t LowerVisibleRasterLineNTSC = 263;
constexpr std::uint16_t UpperDisplayRasterLineNTSC = 51;
constexpr std::uint16_t LowerDisplayRasterLineNTSC = 250;


namespace nvic
{

	constexpr std::array<std::uint32_t, 16> ColorPalette
	{
		0xff000000,			// Black
		0xffffffff,			// White
		0xff444e9f,			// Red
		0xffc6bf6a,			// Cyan
		0xffa357a0,			// Magenta
		0xff5eab5c,			// Green
		0xff9b4550,			// Blue
		0xff87d4c9,			// Yellow
		0xff3c68a1,			// Orange
		0xff12546d,			// Brown
		0xff757ecb,			// Bright red
		0xff626262,			// Dark blue
		0xff898989,			// Grey
		0xff9be29a,			// Bright green
		0xffcb7e88,			// Bright blue
		0xffadadad,			// Bright grey
	};

	VIC_II::VIC_II(MemoryChunk& ioMemory, std::array<MemoryChunk, 16>& ram, Signals& signals)
		: m_IOMemory(ioMemory), m_RAM(ram), m_Signals(signals)
	{
		// Set default values
		m_IOMemory.Data[VIC_ControlRegister1] = 0x9B;
		m_IOMemory.Data[VIC_ControlRegister2] = 0x08;
		m_IOMemory.Data[VIC_MemoryPointers] = 0x14;
		m_IOMemory.Data[VIC_InterruptRegister] = 0x0F;
	
	
//		m_Pixels.resize(320 * 200 * 4);
		m_Pixels.resize(418 * 251);

//		std::memset(m_Pixels.data(), 0, 320 * 200 * 4);
		std::memset(m_Pixels.data(), 0, 418 * 251);
	}


	auto VIC_II::Start() -> void
	{
		m_Running.store(true);
		m_Thread = std::jthread(&VIC_II::ThreadFunc, this);
	}

	auto VIC_II::Stop() -> void
	{
		m_Running.store(false);
		m_Thread.join();
	}

	auto VIC_II::ThreadFunc() -> void
	{

	}

	auto VIC_II::Execute() -> void
	{
//		std::this_thread::sleep_for(3s);

//		std::println("VIC-II is hijacking address enable control signal");

//		std::uint64_t currentCycle{};
//		std::uint64_t lastCycle{};
//		std::uint16_t rasterLine{};

//		while (m_Running)
//		{
//			do
//			{
//				currentCycle = m_Clock.CPUClockCycles();
//			} while (currentCycle == lastCycle);

//			lastCycle = currentCycle;


//			if (m_Signals.Control & !Signal_ClockPhase1 && !m_Signals.BusAvailable)
//			{
//				m_Signals.BusLock = true;







//				m_Signals.BusLock = false;

//				m_Signals.BusAvailable = true;
//			}

		if (m_BadLine)
		{

			m_BadLineCounter++;

			if (m_BadLineCounter == 40)
			{
				m_BadLine = false;
				m_BadLineCounter = 0;

				m_Signals.BusAvailable = true;
			}
			else
			{
				return;
			}
		}

		std::uint8_t borderColorIndex = m_IOMemory.Data[VIC_BorderColor];
		std::uint8_t backgroundColorIndex1 = m_IOMemory.Data[VIC_BackgroundColor0];

		if (m_RasterLine >= UpperVisibleRasterLineNTSC && m_RasterLine <= LowerVisibleRasterLineNTSC)//250)
			m_VisibleRasterLine = true;
		else
			m_VisibleRasterLine = false;

//		if (m_CycleCounter >= 63)
		if (m_CycleCounter >= 60)
		{
			// Sprite fetch and line wrap
			m_CycleCounter = 0;
			m_RasterLine++;

			if (m_RasterLine == 262)
				m_RasterLine = 0;

			if ((m_RasterLine == 0) || ((m_RasterLine % 8) == 0))
			{
				m_BadLine = true;
				m_BadLineCounter = 0;

				m_Signals.BusAvailable = false;
//				std::println("Raster start: {}", m_RasterLine);
			}

		}
		else if (m_CycleCounter >= 56)
		{
			// Right border
			if (m_VisibleRasterLine)
			{
				for (auto i = 0; i < 8; i++)
					m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
			}

		}
		else if (m_CycleCounter >= 16)
		{
			// Main screen area

			if (m_VisibleRasterLine)
			{
				if (m_RasterLine >= UpperDisplayRasterLineNTSC && m_RasterLine <= LowerDisplayRasterLineNTSC)
				{
					// Main display area

					for (auto i = 0; i < 8; i++)
						m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[backgroundColorIndex1];
				}
				else
				{
					for (auto i = 0; i < 8; i++)
						m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
				}

			}
		}
		else if (m_CycleCounter >= 12)
		{
			// Left border
			if (m_VisibleRasterLine)
			{
				for (auto i = 0; i < 8; i++)
					m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
			}
		}
//		if (m_RasterLine == 312)

		m_CycleCounter++;

		m_IOMemory.Data[VIC_RasterCounter] = static_cast<std::uint8_t>(m_RasterLine & 0xFF);

		if (m_RasterLine & 0x100)
			m_IOMemory.Data[VIC_ControlRegister1] |= (1 << 7);
		else
			m_IOMemory.Data[VIC_ControlRegister1] &= ~(1 << 7);

//			m_Signals.Control &= ~Signal_AddressEnableControl;

//		VBlank PAL
//			• Lines 0 – 15: Vertical Blanking period.
//			• Lines 16 – 50 : Top Border area.
//			• Lines 51 – 250 : Main active screen area(200 vertical pixels / 25 rows).
//			• Lines 251 – 299 : Bottom Border area.
//			• Lines 300 – 311 : Vertical Blanking area before resetting to line 0.


//		Anatomy of a Single Raster Line(PAL - 63 Cycles)
//
//			Every raster line is split into exact phases determined by the internal cycle counter(0 to 62) :
//			• Cycles 1 – 11 (Horizontal Blanking & Sync) :
//			• Cycles 1–4 : The horizontal sync pulse is generated.
//			• Cycles 1–9 : Main Horizontal Blanking period(pixels are forced to black).
//			• Cycles 12 – 15 (Left Border Transition) : The VIC - II checks if it needs to start rendering the border.The border drops at cycle 12 in 40 - column mode, or cycle 13 in 38 - column mode.
//			• Cycles 16 – 55 (Main Text / Graphics Window) :
//			• The 40 columns of background / foreground data are fetched and shifted out.
//			• Bad Lines : On the first line of a text row(where the lower 3 bits of $D011 match the lower 3 bits of $D012), the VIC - II triggers a Bad Line.It pulls the CPU's BA (Bus Available) pin low during cycles 12–54, completely halting the CPU during cycles 15–54 to fetch 40 bytes of character pointers from the video matrix.
//			• Cycles 56 – 59 (Right Border Transition) : The right border opens at cycle 56 (40 - column mode) or cycle 55 (38 - column mode).
//			• Cycles 60 – 63 / 0 (Sprite Fetching & Line Wrap) : The VIC - II prepares sprite data for the next raster line.

//		}

	}

	auto VIC_II::Update() -> void
	{
		// Invert select
		m_RAMSelect = 3 - m_IOMemory.Data[VIC_MemoryBankRegister] & VIC_MemoryBanks;

		m_BaseAddress = m_IOMemory.Data[VIC_MemoryPointers];
	}

}
