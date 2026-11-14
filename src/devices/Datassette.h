#pragma once

#include "cpu/Signals.h"


namespace nvic
{

	class Datassette
	{
	public:
		Datassette(Signals& signals);

	private:
		Signals& m_Signals;
	};


}
