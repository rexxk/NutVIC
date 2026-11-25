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

constexpr std::uint16_t UpperVisibleRasterLinePAL = 0;
constexpr std::uint16_t LowerVisibleRasterLinePAL = 300;
constexpr std::uint16_t UpperDisplayRasterLinePAL = 51;
constexpr std::uint16_t LowerDisplayRasterLinePAL = 250;



namespace nvic
{

	constexpr std::array<std::uint16_t, 4> BankAddress
	{
		0x0000,
		0x4000,
		0x8000,
		0xC000,
	};

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

	VIC_II::VIC_II(MemoryChunk& ioMemory, MemoryChunk& charrom, std::array<nvic::MemoryChunk, 16>& ram, Signals& signals, Registers& registers)
		: m_IOMemory(ioMemory), m_CharROM(charrom), m_RAM(ram), m_Signals(signals), m_Registers(registers)
	{
		// Set default values
		m_IOMemory.Data[VIC_ControlRegister1] = 0x9B;
		m_IOMemory.Data[VIC_ControlRegister2] = 0x08;
		m_IOMemory.Data[VIC_MemoryPointers] = 0x14;
		m_IOMemory.Data[VIC_InterruptRegister] = 0x0F;

		m_CharCache.resize(40);
	
//		m_Pixels.resize(320 * 200 * 4);
		// NTSC 6567R8
//		m_Pixels.resize(418 * 251);
		// PAL-B
		m_Pixels.resize(403 * 312);

//		std::memset(m_Pixels.data(), 0, 320 * 200 * 4);
//		std::memset(m_Pixels.data(), 0, 418 * 251);
		std::memset(m_Pixels.data(), 0, 403 * 312);
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
		auto& vicControlRegister1 = m_IOMemory.Data[VIC_ControlRegister1];
		auto& vicInterruptRegister = m_IOMemory.Data[VIC_InterruptRegister];
		auto& vicInterruptEnabled = m_IOMemory.Data[VIC_InterruptEnabled];
		auto& vicMemoryPointers = m_IOMemory.Data[VIC_MemoryPointers];
		auto& vicControlRegister2 = m_IOMemory.Data[VIC_ControlRegister2];
		bool screenEnable = vicControlRegister1 & 0x10;

		m_ScreenMode = 0;
		m_ScreenMode |= (vicControlRegister2 & 0x10) ? 1 << 0 : 0;
		m_ScreenMode |= (vicControlRegister1 & 0x20) ? 1 << 1 : 0;
		m_ScreenMode |= (vicControlRegister1 & 0x40) ? 1 << 2 : 0;

		// VIC graphics accesses
		// 1 - video matrix (c-access, 12 bits)
		// 2 - pixel data (g-access, 8 bits)
		// 3 - sprite data pointers (p-access, 8 bits)
		// 4 - sprite data (s-access, 8 bits)




		std::uint16_t screenRAMPointer = BankAddress[m_RAMSelect] + (m_IOMemory.Data[VIC_MemoryPointers] >> 4) * 0x400;
		std::uint16_t charsetDataPointer = BankAddress[m_RAMSelect] + (((m_IOMemory.Data[VIC_MemoryPointers] >> 1) & 0x7) * 0x800);

		std::uint16_t currentRasterLine = (m_RasterLine - UpperDisplayRasterLinePAL);

		std::uint16_t bitmapDataOffset = vicMemoryPointers & 0x08 ? 0x2000 : 0x0000;

//		if (m_RasterLine >= UpperVisibleRasterLineNTSC && m_RasterLine <= LowerVisibleRasterLineNTSC)
		if (m_RasterLine >= UpperVisibleRasterLinePAL && m_RasterLine <= LowerVisibleRasterLinePAL)
			m_VisibleRasterLine = true;
		else
			m_VisibleRasterLine = false;
		
		if (m_BadLine)
		{
			if (!m_Signals.Executing)
			{
				m_VideoMemoryLineIndex++;

				if (m_VideoMemoryLineIndex > 40)
				{
					m_BadLine = false;
					m_VideoMemoryLineIndex = 0;

					m_Signals.BusAvailable = true;
				}
				else
				{
					// Perform bad line operations (i.e read text line data)
					// TODO(da): Bad line start is also dependent of YScroll variable, that is not
					// implemented at the moment.

					// Address generator - start by calculating c-address
					m_VideoCounterBase = bitmapDataOffset + (currentRasterLine / 8 * 40) & 0x3FF;
					m_VideoCounter = m_VideoCounterBase + (m_VideoMemoryLineIndex - 1);

					// VM13/VM12/VM11/VM10
					m_CAddress = (vicMemoryPointers & 0xF0) << 6;
					m_CAddress |= m_VideoCounter;

					m_CAddress += BankAddress[m_RAMSelect];

					std::uint8_t cChunk = m_CAddress >> 12;
					std::uint16_t cAddress = m_CAddress & 0x0FFF;

					// Read out the location of the 8x8/4x8 chunk in character/data memory
					std::uint8_t ramLocation = m_RAM.at(cChunk).Data[cAddress];

					m_CAccessData = ramLocation;

					switch (m_ScreenMode)
					{
						case VIC_ScreenModeStandardCharacter:
						{
							// Calculate g-address
							m_GAddress = (vicMemoryPointers & 0x0E) << 10;
							m_GAddress |= ramLocation << 3;
							m_GAddress |= (currentRasterLine & 0x7);							
							m_GAddress += BankAddress[m_RAMSelect];

							std::uint8_t gChunk = m_GAddress >> 12;
							std::uint16_t gAddress = m_GAddress & 0x0FFF;

							for (auto i = 0; i < 8; i++)
							{
								CG cgData{};
								cgData.CData = (m_IOMemory.Data[0x800 + m_VideoCounter] << 8) + ramLocation;

								if (gChunk == ((BankAddress[m_RAMSelect] >> 12) + 1) && !(m_RAMSelect & 0x01))
									cgData.GData = m_CharROM.Data[gAddress + i];
								else
									cgData.GData = m_RAM.at(gChunk).Data[gAddress + i];

								m_CharCache[m_VideoMemoryLineIndex - 1][i] = cgData;
							}

							break;
						}

						case VIC_ScreenModeMulticolorCharacter:
						{
							// Calculate g-address
							m_GAddress = (vicMemoryPointers & 0x0E) << 10;
							m_GAddress |= ramLocation << 3;
							m_GAddress |= (currentRasterLine & 0x7);
							m_GAddress += BankAddress[m_RAMSelect];

							std::uint8_t gChunk = m_GAddress >> 12;
							std::uint16_t gAddress = m_GAddress & 0x0FFF;

							for (auto i = 0; i < 8; i++)
							{
								CG cgData{};

//								auto vcr2 = vicControlRegister2 & 0x10;
//								auto col = m_IOMemory.Data[0x800 + m_VideoCounter] & 0x07;
								// 11 MC 10-8 col 7-0 data

//								0x1000'0000'0000
//								0x0000'0001'0000

								if (vicControlRegister2 & 0x10)
								{
									cgData.CData = 0x800;
								}

								cgData.CData = ((vicControlRegister2 & 0x10) << 7) + ((m_IOMemory.Data[0x800 + m_VideoCounter] & 0x07) << 8) + ramLocation;

								if ((gChunk == ((BankAddress[m_RAMSelect] >> 12) + 1)) && !(m_RAMSelect & 0x01))
									cgData.GData = m_CharROM.Data[gAddress + i];
								else
									cgData.GData = m_RAM.at(gChunk).Data[gAddress + i];

								m_CharCache[m_VideoMemoryLineIndex - 1][i] = cgData;
							}

							break;
						}

						case VIC_ScreenModeStandardBitmap:
						{
							// Calculate g-address
							m_GAddress = (vicMemoryPointers & 0x08) << 10;
							m_GAddress |= (m_VideoCounter & 0x3FF) << 3;
							m_GAddress |= (currentRasterLine & 0x7);
							m_GAddress += BankAddress[m_RAMSelect];

							break;
						}

						case VIC_ScreenModeMulticolorBitmap:
						{
							// Calculate g-address
							m_GAddress = (vicMemoryPointers & 0x08) << 10;
							m_GAddress |= (m_VideoCounter & 0x3FF) << 3;
							m_GAddress |= (currentRasterLine & 0x7);
							m_GAddress += BankAddress[m_RAMSelect];

							// Calculate g-address
//							10'0000'0000'0000
//							00'0000'0000'1000

							if (vicMemoryPointers & 0x08)
							{
								m_GAddress = (vicMemoryPointers & 0x08) << 10;
							}

							m_GAddress = (vicMemoryPointers & 0x08) << 10;
							m_GAddress |= (m_VideoCounter & 0x3FF) << 3;
							m_GAddress |= (currentRasterLine & 0x7);
							m_GAddress += BankAddress[m_RAMSelect];

							std::uint8_t gChunk = m_GAddress >> 12;
							std::uint16_t gAddress = m_GAddress & 0x0FFF;

							for (auto i = 0; i < 8; i++)
							{
								CG cgData{};
								cgData.CData = (m_IOMemory.Data[0x800 + m_VideoCounter] << 8) + ramLocation;

								if (gChunk == ((BankAddress[m_RAMSelect] >> 12) + 1) && !(m_RAMSelect & 0x01))
									cgData.GData = m_CharROM.Data[gAddress + i];
								else
									cgData.GData = m_RAM.at(gChunk).Data[gAddress + i];

								m_CharCache[m_VideoMemoryLineIndex - 1][i] = cgData;
							}

							break;
						}

						case VIC_ScreenModeExtendedBackgroundColor:
						{
							__debugbreak();
							break;
						}
					}

//					m_GAddress += BankAddress[m_RAMSelect];

//					std::uint8_t gChunk = m_GAddress >> 12;
//					std::uint16_t gAddress = m_GAddress & 0x0FFF;
//					std::uint16_t colormapData = m_IOMemory.Data[0x800 + m_VideoCounter];

//					for (auto i = 0; i < 8; i++)
//						m_CharCache[m_VideoMemoryLineIndex - 1][i] = cgData;

					return;
				}

			}
		}

		std::uint8_t borderColorIndex = m_IOMemory.Data[VIC_BorderColor] & 0x0F;
		std::uint8_t backgroundColorIndex1 = m_IOMemory.Data[VIC_BackgroundColor0] & 0x0F;
		std::uint8_t backgroundColorIndex2 = m_IOMemory.Data[VIC_BackgroundColor1] & 0x0F;
		std::uint8_t backgroundColorIndex3 = m_IOMemory.Data[VIC_BackgroundColor2] & 0x0F;

//		if (m_CycleCounter >= 63)
		if (m_CycleCounter >= m_RowAdjust)
//		if (m_CycleCounter >= 60)
		{
			// Sprite fetch and line wrap
			m_CycleCounter = 0;
			m_RasterLine++;

//			if (m_RasterLine == 262)
			if (m_RasterLine == 311)
				m_RasterLine = 0;

//			if (((m_RasterLine == UpperDisplayRasterLineNTSC) || (((m_RasterLine - UpperDisplayRasterLineNTSC) % 8) == 0)
//				&& (m_RasterLine >= UpperDisplayRasterLineNTSC)) && (m_RasterLine < LowerDisplayRasterLineNTSC) && screenEnable)
			if (((m_RasterLine == UpperDisplayRasterLinePAL) || (((m_RasterLine - UpperDisplayRasterLinePAL) % 8) == 0)
				&& (m_RasterLine >= UpperDisplayRasterLinePAL)) && (m_RasterLine < LowerDisplayRasterLinePAL) && screenEnable)
			{
				m_BadLine = true;
				m_VideoMemoryLineIndex = 0;

				m_Signals.BusAvailable = false;
			}

		}
		else if (m_CycleCounter >= 56)
		{
			// Right border
			if (m_VisibleRasterLine)
			{
				for (auto i = 0; i < 8; i++)
					m_Pixels[((m_CycleCounter - 12) * 8) + i + ((m_RasterLine - UpperVisibleRasterLinePAL) * 403)] = ColorPalette[borderColorIndex];
//					m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
			}

		}
		else if (m_CycleCounter >= 16)
		{
			// Main screen area

			if (m_VisibleRasterLine)
			{
//				if (m_RasterLine >= UpperDisplayRasterLineNTSC && m_RasterLine <= LowerDisplayRasterLineNTSC && screenEnable)
				if (m_RasterLine >= UpperDisplayRasterLinePAL && m_RasterLine <= LowerDisplayRasterLinePAL && screenEnable)
				{
					// Main display area

					switch (m_ScreenMode)
					{
						case VIC_ScreenModeStandardCharacter:
						{
							for (auto i = 0; i < 8; i++)
							{
								auto pixel = m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].GData & (1 << (7 - i));
								std::uint8_t color = (m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].CData >> 8) & 0x0F;
								m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = pixel ? ColorPalette[color] : ColorPalette[backgroundColorIndex1];
							}

							break;
						}

						case VIC_ScreenModeMulticolorCharacter:
						{
							std::uint8_t color{};

							if (m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].CData & 0x800)
							{
								for (auto i = 0; i < 4; i++)
								{
									auto value = (m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].CData >> 8) & 0x07;
									auto pixel = ((m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].GData & 0xFF) >> ((3 - i) * 2) & 0x03);

									if (pixel == 0) color = backgroundColorIndex1;
									if (pixel == 1) color = backgroundColorIndex2;
									if (pixel == 2) color = backgroundColorIndex3;
									if (pixel == 3) color = value;

									m_Pixels[((m_CycleCounter - 12) * 8) + (i * 2) + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[color];
									m_Pixels[((m_CycleCounter - 12) * 8) + ((i * 2) + 1) + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[color];
								}
							}
							else
							{
								for (auto i = 0; i < 8; i++)
								{
									auto pixel = m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].GData & (1 << (7 - i));
									std::uint8_t color = (m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].CData >> 8) & 0x07;
									m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = pixel ? ColorPalette[color] : ColorPalette[backgroundColorIndex1];
								}
							}

							break;
						}

						case VIC_ScreenModeStandardBitmap:
						{
//							__debugbreak();

//							for (auto i = 0; i < 8; i++)
//							{
//								auto value = m_CharCache[m_CycleCounter - 16][currentRasterLine % 8];
//								auto pixel = m_CharCache[m_CycleCounter - 16][currentRasterLine % 8] & (1 << (7 - i));
//
//								m_Pixels[((m_CycleCounter - 12) * 8) + (i * 2) + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = pixel ? ColorPalette[(value >> 20) & 0x0F] : ColorPalette[(value >> 16) & 0x0F];
//							}

							break;
						}

						case VIC_ScreenModeMulticolorBitmap:
						{
							std::uint8_t color{};

							for (auto i = 0; i < 4; i++)
							{
								auto value = m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].CData;
								auto pixel = ((m_CharCache[m_CycleCounter - 16][currentRasterLine % 8].GData & 0xFF) >> ((3 - i) * 2) & 0x03);

								if (pixel == 0) color = m_IOMemory.Data[0x021] & 0x0F;
								if (pixel == 1) color = (value >> 4) & 0x0F;
								if (pixel == 2) color = (value) & 0x0F;
								if (pixel == 3) color = (value >> 8) & 0x0F;

								m_Pixels[((m_CycleCounter - 12) * 8) + (i * 2) + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[color];
								m_Pixels[((m_CycleCounter - 12) * 8) + ((i * 2) + 1) + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[color];
							}

							break;
						}

						case VIC_ScreenModeExtendedBackgroundColor:
						{
							__debugbreak();
							break;
						}
					}

//					// Check for bitmap or text mode?
//					if (vicControlRegister1 & 0x20)
//					{
//					}
				}
				else
				{
					for (auto i = 0; i < 8; i++)
						m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[borderColorIndex];
//						m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
				}

			}
		}
		else if (m_CycleCounter >= 12)
		{
			// Left border
			if (m_VisibleRasterLine)
			{
				for (auto i = 0; i < 8; i++)
					m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLinePAL) * 403] = ColorPalette[borderColorIndex];
//					m_Pixels[((m_CycleCounter - 12) * 8) + i + (m_RasterLine - UpperVisibleRasterLineNTSC) * 418] = ColorPalette[borderColorIndex];
			}
		}
		else if (m_CycleCounter == 1)
		{
			m_RasterInterruptLine = (m_Registers.VIC_RasterInterruptHighByte << 8) + m_Registers.VIC_RasterInterruptLowByte;

			// Trigger raster interrupt
			if ((m_RasterLine == m_RasterInterruptLine) && (vicInterruptEnabled & 0x01))
			{
				vicInterruptRegister |= 0x81;
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

		m_PendingInterrupt = vicInterruptRegister & 0x80;
	}

	auto VIC_II::Update() -> void
	{
		// Invert select
		m_RAMSelect = 3 - m_IOMemory.Data[VIC_MemoryBankRegister] & VIC_MemoryBanks;

		m_BaseAddress = m_IOMemory.Data[VIC_MemoryPointers];
	}

}
