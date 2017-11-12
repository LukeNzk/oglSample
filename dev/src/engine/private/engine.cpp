#include "engine.h"
#include "..\..\utils\public\window.h"

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
	}
}
