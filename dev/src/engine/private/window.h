#pragma once
#include "../../utils/public/types.h"

class IInputManager;
class WindowProcedure;

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	void SetInputManager( IInputManager* inputManager );
	Bool Create( Uint32 width, Uint32 height );
	void Tick();

	void* GetNativeHandle() { return m_hWnd; }

private:
	void*				m_hInstance;
	void*				m_hWnd;
	WindowProcedure*	m_winProc;

	Uint32 m_width;
	Uint32 m_height;
};