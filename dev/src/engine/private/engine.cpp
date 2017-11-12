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
	graphics::Init( const_cast< void* >( m_window->GetNativeHandle() ) );
}

void Engine::Start()
{
	// update loop
	while ( !m_quit )
	{
		
		m_window->Tick();
	}
}
