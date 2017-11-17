#pragma once
#include "../../utils/public/types.h"

#include <vector>

class TextureManager;
class Sprite;
class Missile;
struct Vector2;

class MissilesManager
{
public:
	MissilesManager();
	~MissilesManager();

	void LoadResources( TextureManager* textures );
	void Tick( Float dt );
	void TryShoot( const Vector2& position, const Vector2& direction );

private:
	std::vector< Missile* > m_missiles;
	std::vector< Sprite* > m_spritesPool;

	Float m_timeSinceLastShot;

	const Float c_missileLifetime = 1.0f;
	const Float c_shootingInterval = 0.5f;
	const Float c_maxMissiles = c_missileLifetime / c_shootingInterval;
};