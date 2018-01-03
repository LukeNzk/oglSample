#include "game.h"

#include "..\..\engine\public\engine.h"
#include "..\..\engine\public\textureManager.h"
#include "..\..\engine\public\sprite.h"
#include "..\..\engine\public\input.h"

#include "../../utils/public/vector2.h"
#include "../../utils/public/debug.h"

extern Engine* GEngine;

Game::Game()
{
}

Game::~Game()
{
}

void Game::LoadResources()
{
	const Uint32 data[ 4 ] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff00 };
	GEngine->SetOverlayData( 2, 2, data );
}

void Game::Tick( Float dt )
{
}

void Game::DispatchEvent( ERIEventType type, void* data )
{
}
