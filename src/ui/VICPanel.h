#pragma once

#include "chipset/VIC_II.h"


namespace nvic
{

	class VICPanel
	{
	public:
		VICPanel(VIC_II& vic);

		auto Draw() -> void;

	private:
		VIC_II& m_VIC;
	};

}
