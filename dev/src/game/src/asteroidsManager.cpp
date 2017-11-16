#include "asteroidsManager.h"
#include "../../engine/public/engine.h"
#include "../../engine/public/sprite.h"
#include "../../engine/public/textureManager.h"

extern Engine* GEngine;

AsteroidsManager::AsteroidsManager()
	: m_maxAsteroids( 10 )
{
}

AsteroidsManager::~AsteroidsManager()
{
	for ( Asteroid* asteroid : m_asteroids )
	{
		delete asteroid;
	}

	m_asteroids.clear();
	m_spritesPool.clear();
}

void AsteroidsManager::LoadResources( TextureManager* textures )
{
	const AnsiChar* asteroidPng = "asteroid.png";
	textures->LoadTexture( asteroidPng );

	Sprite* spr = nullptr;
	const Uint32 texture = textures->FindTexture( asteroidPng );

	for ( Uint32 i = 0; i < m_maxAsteroids; ++i )
	{
		spr = GEngine->CreateSprite();
		spr->m_position = Vector2( 1000, 1000 );
		m_asteroids.push_back( new Asteroid );
	}
}

void AsteroidsManager::Spawn()
{
	
}