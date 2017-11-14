#include "..\..\engine\public\engine.h"
#include "..\..\utils\public\macros.h"
#include "..\..\engine\public\game.h"

#include <stdlib.h>

Engine* GEngine = nullptr;

class Game final : public IGame 
{
public:
	~Game() override { }

	void Tick( Float dt ) override
	{

	}

private:
};

void main()
{
	Game game;
	Engine engine;
	GEngine = &engine;

	GEngine->Init();
	GEngine->Start( &game );
}