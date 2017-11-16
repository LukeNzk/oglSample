#pragma once
#include "../../engine/public/input.h"

class InputManager : IInputManager
{
public:
	// Inherited via IInputManager
	virtual void DispatchEvent( ERIEventType type, void* data ) override;
	
};