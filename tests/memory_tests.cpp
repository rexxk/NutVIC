#include <gtest/gtest.h>

#include "cpu/Signals.h"
#include "memory/Memory.h"

#include <array>
#include <cstdint>


TEST(MemoryTests, ReadWriteData)
{
	nvic::Signals signals{};
	nvic::Memory memory{ signals };

	std::array<nvic::MemoryChunk, 4> chunks{};
	
	for (std::uint8_t i = 0; i < 4; i++)
		memory.SetChunk(i, chunks.at(i));

	signals.AddressBus = 0x00FF;
	signals.DataBus = 0x3F;

	memory.Write();
	memory.Read();

	ASSERT_EQ(signals.DataBus, 0x3F);

	signals.AddressBus = 0x40FF;
	signals.DataBus = 0x7F;

	memory.Write();
	memory.Read();

	ASSERT_EQ(signals.DataBus, 0x7F);

	signals.AddressBus = 0x91FF;
	signals.DataBus = 0xBF;

	memory.Write();
	memory.Read();

	ASSERT_EQ(signals.DataBus, 0xBF);

	signals.AddressBus = 0xE7FF;
	signals.DataBus = 0xFF;

	memory.Write();
	memory.Read();

	ASSERT_EQ(signals.DataBus, 0xFF);
}	

TEST(MemoryTests, WriteToROM)
{
	nvic::Signals signals{};
	nvic::Memory memory{ signals };

	nvic::MemoryChunk chunk{ .ReadWrite{false} };

	memory.SetChunk(0, chunk);

	signals.AddressBus = 0x00FF;
	signals.DataBus = 0x3F;

	memory.Write();
	memory.Read();

	ASSERT_EQ(signals.DataBus, 0x00);
}
