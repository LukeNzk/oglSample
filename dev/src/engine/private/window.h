#pragma once
#include "../../utils/public/types.h"

class IInputManager;

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

	void SetInputManager( IInputManager* inputManager );
	Bool Create( Uint32 width, Uint32 height );
	void Tick();

	void* GetNativeHandle() { return m_hWnd; }
};