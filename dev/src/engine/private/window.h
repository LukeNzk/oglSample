#pragma once
#include "../../utils/public/types.h"

class IInputManager;
class WindowProcedure;

class CWindow
{
	friend class CEngine;

public:
	CWindow();
	~CWindow();

	void SetInputManager( IInputManager* inputManager );
	Bool Create( Uint32 width, Uint32 height );
	void Tick();

	void* GetNativeHandle() { return m_hWnd; }

private:
	void*				m_hInstance;
	void*				m_hWnd;
	WindowProcedure*	m_winProc;

	int m_width;
	int m_height;
};