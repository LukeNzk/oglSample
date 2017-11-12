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

	int m_width;
	int m_height;

public:
	CWindow();
	CWindow( Input::IInputManager* inputManager );
	Bool Create( Uint32 width, Uint32 height );
	void Tick();
};