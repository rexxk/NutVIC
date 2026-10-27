#pragma once

#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	class VIC_II
	{
	public:
		VIC_II(MemoryChunk& memory, Signals& signals);

	private:
		MemoryChunk& m_Memory;
		Signals& m_Signals;


	};

}
