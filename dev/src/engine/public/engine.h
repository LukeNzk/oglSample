#pragma once
#include "../../utils/public/types.h"
#include <memory>

class CWindow;
typedef std::unique_ptr< CWindow > WndPtrU;

class Engine
{
public:
	Engine();
	~Engine();

	void Init();
	void Start();

private:
	WndPtrU m_window;
	Bool	m_quit;
};