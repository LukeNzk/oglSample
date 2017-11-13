#pragma once
#include "../../utils/public/types.h"

#include <memory>

class CWindow;
class Renderer;

typedef std::unique_ptr< CWindow > WndPtrU;
typedef std::unique_ptr< Renderer > RendererPtrU;

class Engine
{
public:
	Engine();
	~Engine();

	void Init();
	void Start();

private:
	WndPtrU			m_window;
	RendererPtrU	m_renderer;
	Bool			m_quit;
};