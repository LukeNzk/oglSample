#pragma once
#include "../../utils/public/types.h"

#include <memory>

class CWindow;
class Renderer;
class IGame;

typedef std::unique_ptr< CWindow > WndPtrU;
typedef std::unique_ptr< Renderer > RendererPtrU;

class Engine
{
public:
	Engine();
	~Engine();

	void Init();
	void Start( IGame* game );

private:
	WndPtrU			m_window;
	IGame*			m_game;
	RendererPtrU	m_renderer;
	Bool			m_quit;
};