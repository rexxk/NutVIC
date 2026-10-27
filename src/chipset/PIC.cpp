#include "chipset/PIC.h"



namespace nvic
{

	PIC::PIC(std::uint8_t id, MemoryChunk& memoryChunk, Signals& signals)
		: m_ID(id), m_IOMemory(memoryChunk), m_Signals(signals)
	{
		m_BaseAddress = 0xC00 + id * 0x100;

	}



}
