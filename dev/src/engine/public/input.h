#pragma once
#include "inputCodes.h"

class IInputManager
{
public:
	virtual void DispatchEvent( ERIEventType type, void* data ) = 0;
};