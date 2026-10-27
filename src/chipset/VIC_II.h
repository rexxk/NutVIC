#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <atomic>
#include <cstdint>
#include <thread>


namespace nvic
{


	constexpr std::uint8_t VIC_Sprite0CoordinateX = 0x00;
	constexpr std::uint8_t VIC_Sprite0CoordinateY = 0x01;
	constexpr std::uint8_t VIC_Sprite1CoordinateX = 0x02;
	constexpr std::uint8_t VIC_Sprite1CoordinateY = 0x03;
	constexpr std::uint8_t VIC_Sprite2CoordinateX = 0x04;
	constexpr std::uint8_t VIC_Sprite2CoordinateY = 0x05;
	constexpr std::uint8_t VIC_Sprite3CoordinateX = 0x06;
	constexpr std::uint8_t VIC_Sprite3CoordinateY = 0x07;
	constexpr std::uint8_t VIC_Sprite4CoordinateX = 0x08;
	constexpr std::uint8_t VIC_Sprite4CoordinateY = 0x09;
	constexpr std::uint8_t VIC_Sprite5CoordinateX = 0x0A;
	constexpr std::uint8_t VIC_Sprite5CoordinateY = 0x0B;
	constexpr std::uint8_t VIC_Sprite6CoordinateX = 0x0C;
	constexpr std::uint8_t VIC_Sprite6CoordinateY = 0x0D;
	constexpr std::uint8_t VIC_Sprite7CoordinateX = 0x0E;
	constexpr std::uint8_t VIC_Sprite7CoordinateY = 0x0F;
	constexpr std::uint8_t VIC_SpriteCoordinateXMSB = 0x10;
	constexpr std::uint8_t VIC_ControlRegister1 = 0x11;
	constexpr std::uint8_t VIC_RasterCounter = 0x12;
	constexpr std::uint8_t VIC_LightPenX = 0x13;
	constexpr std::uint8_t VIC_LightPenY = 0x14;
	constexpr std::uint8_t VIC_SpriteEnabled = 0x15;
	constexpr std::uint8_t VIC_ControlRegister2 = 0x16;
	constexpr std::uint8_t VIC_SpriteYExpansion = 0x17;
	constexpr std::uint8_t VIC_MemoryPointers = 0x18;
	constexpr std::uint8_t VIC_InterruptRegister = 0x19;
	constexpr std::uint8_t VIC_InterruptEnabled = 0x1A;
	constexpr std::uint8_t VIC_SpriteDataPriority = 0x1B;
	constexpr std::uint8_t VIC_SpriteMulticolor = 0x1C;
	constexpr std::uint8_t VIC_SpriteXExpansion = 0x1D;
	constexpr std::uint8_t VIC_SpriteSpriteCollision = 0x1E;
	constexpr std::uint8_t VIC_SpriteDataCollision = 0x1F;
	constexpr std::uint8_t VIC_BorderColor = 0x20;
	constexpr std::uint8_t VIC_BackgroundColor0 = 0x21;
	constexpr std::uint8_t VIC_BackgroundColor1 = 0x22;
	constexpr std::uint8_t VIC_BackgroundColor2 = 0x23;
	constexpr std::uint8_t VIC_BackgroundColor3 = 0x24;
	constexpr std::uint8_t VIC_SpriteMulticolor0 = 0x25;
	constexpr std::uint8_t VIC_SpriteMulticolor1 = 0x26;
	constexpr std::uint8_t VIC_ColorSprite0 = 0x27;
	constexpr std::uint8_t VIC_ColorSprite1 = 0x28;
	constexpr std::uint8_t VIC_ColorSprite2 = 0x29;
	constexpr std::uint8_t VIC_ColorSprite3 = 0x2A;
	constexpr std::uint8_t VIC_ColorSprite4 = 0x2B;
	constexpr std::uint8_t VIC_ColorSprite5 = 0x2C;
	constexpr std::uint8_t VIC_ColorSprite6 = 0x2D;
	constexpr std::uint8_t VIC_ColorSprite7 = 0x2E;


	class VIC_II
	{
	public:
		VIC_II(MemoryChunk& memory, Signals& signals);

		auto Start() -> void;
		auto Stop() -> void;

	private:
		void ThreadFunc();

	private:
		MemoryChunk& m_Memory;
		Signals& m_Signals;

		std::atomic<bool> m_Running{ false };

		std::jthread m_Thread;
	};

}
