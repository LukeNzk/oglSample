#include "engine.h"
#include "window.h"
#include "graphics.h"
#include "renderer.h"
#include "game.h"
#include "sprite.h"

#include "../../utils/public/timer.h"
#include "../../utils/public/debug.h"
#include "../../utils/public/macros.h"

Engine::Engine()
	: m_quit( false )
	, m_game( nullptr )
	, m_overlaySprite( nullptr )
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

	m_overlaySprite = m_renderer->CreateSprite();
	Uint32 overlayTexture = graphics::CreateTexture2D();
	m_overlaySprite->SetTexture( overlayTexture );
}

void Engine::Start( IGame* game )
{
	m_game = game;

	Timer timer;
	timer.Start();

	m_game->LoadResources();

	Debug::SCDebug::Info( "Resources loaded... %f s\n", timer.TimeElapsed() );
	timer.Start();
	Float dt = 0.0f;

	// update loop
	while ( !m_quit )
	{
		dt = static_cast< Float >( timer.TimeElapsed() );
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

void Engine::SetOverlayData( Uint32 width, Uint32 height, const void* data ) const
{
	SC_ASSERT( m_overlaySprite, "No overlay was initialized." );
	graphics::BindTexture2D( m_overlaySprite->GetTexture() );
	graphics::UploadOverlayTexture( m_overlaySprite->GetTexture(), width, height, data );
	m_overlaySprite->m_scale = { ( Float )width / height, 1.0f };
}