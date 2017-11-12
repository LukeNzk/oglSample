#pragma once
#include "types.h"

namespace Input
{
	class IInputManager;
}

class CWindow
{
	friend class CEngine;

private:
	void* m_hInstance;
	void* m_hWnd;

	int m_width = 800;
	int m_height = 600;

public:
	CWindow();
	CWindow( Input::IInputManager* inputManager );
	Bool Create();
	void Tick();
};