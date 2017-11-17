#include "game.h"
#include "movement.h"
#include "asteroid.h"
#include "asteroidsManager.h"

#include "..\..\engine\public\engine.h"
#include "..\..\engine\public\textureManager.h"
#include "..\..\engine\public\sprite.h"
#include "..\..\engine\public\input.h"

#include "../../utils/public/vector2.h"
#include "../../utils/public/debug.h"

extern Engine* GEngine;

Sprite* spr = nullptr;
Vector2 mousePos;

Game::Game()
	: m_asteroids( nullptr )
{
}

Game::~Game() 
{
	delete m_asteroids;
	m_asteroids = nullptr;
}

void Game::LoadResources()
{
	TextureManager textures;
	textures.LoadTexture( "ship.png" );

	spr = GEngine->CreateSprite();
	spr->SetTexture( textures.FindTexture( "ship.png" ) );
	spr->m_scale = 0.1f;

	m_asteroids = new AsteroidsManager;
	m_asteroids->LoadResources( &textures );
}

Movement mvt;

void Game::Tick( Float dt )
{
	mvt.Move( dt );

	spr->m_position = mvt.m_position;
	spr->m_rotation = mvt.m_rotation;

	m_asteroids->Tick( dt );
}

void Game::DispatchEvent( ERIEventType type, void* data )
{
	if ( type == ERIEventType::RIE_DOWN )
	{
		const Uint32 keyCode = *static_cast< Uint32* >( data );
		if ( keyCode == RI_SPACE )
		{
			mvt.SetThrusters( true );
		}
		else if( keyCode == RI_LMB || keyCode == RI_RMB )
		{
			Debug::SCDebug::Info( "LMB %d\n", keyCode );
		}
	}
	else if ( type == ERIEventType::RIE_UP )
	{
		const Uint32 keyCode = *static_cast< Uint32* >( data );
		if ( keyCode == RI_SPACE )
		{
			mvt.SetThrusters( false );
		}
	}
	else if ( type == ERIEventType::RIE_MOUSEMOVE )
	{
		const Int16* point( reinterpret_cast< const Int16* >( data ) );
		mousePos.x = point[ 0 ];
		mousePos.y = point[ 1 ];
		mvt.SetMousePosition( mousePos );
	}
}
