#include "engine.h"
#include "window.h"
#include "graphics.h"
#include "renderer.h"

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

	m_renderer.reset( new Renderer() );
	m_renderer->Init();
}

void Engine::Start()
{
	// update loop
	while ( !m_quit )
	{
		m_window->Tick();
		m_renderer->Draw();
	}
}
