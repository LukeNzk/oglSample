#include "engine.h"
#include "window.h"
#include "graphics.h"
#include "renderer.h"
#include "game.h"

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

	graphics::Init( m_window->GetNativeHandle() );

	m_renderer.reset( new Renderer() );
	m_renderer->Init();
}

void Engine::Start( IGame* game )
{
	m_game = game;

	// update loop
	while ( !m_quit )
	{
		m_game->Tick( 0.0f );
		m_window->Tick();
		m_renderer->Draw();
	}
}
