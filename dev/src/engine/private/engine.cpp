#include "engine.h"
#include "window.h"
#include "graphics.h"
#include "renderer.h"
#include "game.h"
#include "sprite.h"

#include "../../utils/public/timer.h"
#include "../../utils/public/debug.h"

Engine::Engine()
	: m_quit( false )
	, m_game( nullptr )
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

	Timer< Float > timer;
	timer.Start();

	m_game->LoadResources();

	Debug::SCDebug::Info( "Resources loaded... %f s\n", timer.TimeElapsed() );
	timer.Start();
	Float dt = 0.0f;

	// update loop
	while ( !m_quit )
	{
		dt = timer.TimeElapsed();
		timer.Start();

		m_game->Tick( dt );
		m_renderer->Draw();
		m_window->Tick();
	}
}

Sprite* Engine::CreateSprite()
{
	return m_renderer->CreateSprite();
}

void Engine::SetInputManager( IInputManager* input )
{
	m_window->SetInputManager( input );
}