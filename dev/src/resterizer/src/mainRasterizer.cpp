#include "..\..\utils\public\macros.h"
#include "..\..\engine\public\engine.h"

#include "game.h"

Engine* GEngine = nullptr;

void main()
{
	Game game;
	Engine engine;
	GEngine = &engine;

	GEngine->Init();
	GEngine->SetInputManager( &game );
	GEngine->Start( &game );
}