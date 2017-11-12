#pragma once
#include "../../utils/public/input.h"

class InputManager : Input::IInputManager
{
public:
	// Inherited via IInputManager
	virtual void DispatchEvent( ERIEventType type, void* data ) override;
	
};