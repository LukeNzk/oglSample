#pragma once
#include "inputCodes.h"

namespace Input
{
	class IInputManager
	{
	public:
		virtual void DispatchEvent( ERIEventType type, void* data ) = 0;
	};
}