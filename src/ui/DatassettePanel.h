#pragma once

#include "devices/Datassette.h"


namespace nvic
{

	class DatassettePanel
	{
	public:
		DatassettePanel(Datassette& datassette);

		auto Draw() -> void;

	private:
		Datassette& m_Datassette;

	};

}
