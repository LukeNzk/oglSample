#include "engine.h"
#include "window.h"
#include "graphics.h"

Engine::Engine()
	: m_quit( false )
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
	m_window.reset( new CWindow() );
	m_window->Create( 800, 600 );
}

void Engine::Start()
{
	// update loop
	while ( !m_quit )
	{
		m_window->Tick();
		graphics::SetClearColor( 1, 0, 0, 1 );
		graphics::ClearColorAndDepth();
		graphics::SwapBuffers();
	}
}
