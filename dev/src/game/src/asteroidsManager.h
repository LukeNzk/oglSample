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

private:
	void Spawn();

private:
	std::vector< Asteroid* > m_asteroids;
	std::vector< Sprite* > m_spritesPool;

	Uint32 m_texture;
	Uint32 m_maxAsteroids;
};