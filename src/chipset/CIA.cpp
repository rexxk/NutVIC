#include "chipset/CIA.h"



namespace nvic
{


	CIA::CIA(std::uint8_t id, MemoryChunk& ioMemory, Signals& signals)
		: m_ID(id), m_Memory(ioMemory), m_Signals(signals)
	{

	}


	auto CIA::Update() -> void
	{
		if (m_ID == 1)
		{
			m_Memory.Data[0xD00] |= 0x80;
		}
	}


}
