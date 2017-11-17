#pragma once
#include "asteroid.h"

#include <vector>

class TextureManager;
class Sprite;

class AsteroidsManager
{
public:
	AsteroidsManager();
	~AsteroidsManager();

	void LoadResources( TextureManager* textures );
	void Tick( Float dt );
	Bool TryDestroyAsteroid( const Vector2& point );

private:
	void Spawn();
	void Deactivate( Uint32 asteroidIndex );

private:
	std::vector< Asteroid* > m_asteroids;
	std::vector< Sprite* > m_spritesPool;

	Uint32 m_maxAsteroids;
	Float m_timeSinceLastSpawn;
	const Float m_spawningInterval = 1.0f;
};