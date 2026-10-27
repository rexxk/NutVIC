#include "chipset/VIC_II.h"

#include "cpu/Signals.h"
#include "memory/Memory.h"


namespace nvic
{

	VIC_II::VIC_II(MemoryChunk& memory, Signals& signals)
		: m_Memory(memory), m_Signals(signals)
	{

	}


}
