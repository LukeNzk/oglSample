#include "asteroidsManager.h"
#include "../../engine/public/engine.h"
#include "../../engine/public/sprite.h"
#include "../../engine/public/textureManager.h"

#include "../../utils/public/random.h"

extern Engine* GEngine;

namespace helper
{
	Vector2 RandomOnUnitCircle()
	{
		Vector2 result( Random01f(), Random01f() );
		result -= Vector2( 0.5f, 0.5f );
		if ( result.SqrMagnitude() == 0.0f )
			result.y = 1.0f;
		else
		{
			result.Normalize();
		}

		return result;
	}

	Vector2 RandomInUnitCircle()
	{
		Vector2 result( Random01f(), Random01f() );
		result -= Vector2( 0.5f, 0.5f );
		result *= 2.0f;

		return result;
	}
}

AsteroidsManager::AsteroidsManager()
	: m_maxAsteroids( 10 )
	, m_timeSinceLastSpawn( 0.0f )
{
	RandomSeed( *( Int32* )this );
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

	m_asteroids.reserve( m_maxAsteroids );
	m_spritesPool.reserve( m_maxAsteroids );

	for ( Uint32 i = 0; i < m_maxAsteroids; ++i )
	{
		spr = GEngine->CreateSprite();
		spr->m_position = Vector2( 1000, 1000 );
		spr->SetTexture( texture );

		Asteroid* asteroid = new Asteroid;
		asteroid->m_radius = 0.1f;
		spr->m_scale = 0.1f;

		m_asteroids.push_back( asteroid );
		m_spritesPool.push_back( spr );
	}
}

void AsteroidsManager::Tick( Float dt )
{
	m_timeSinceLastSpawn += dt;
	if ( m_timeSinceLastSpawn >= m_spawningInterval )
	{
		m_timeSinceLastSpawn -= m_spawningInterval;
		Spawn();
	}

	Asteroid* asteroid;
	Sprite* sprite;
	for ( Uint32 i = 0; i < m_maxAsteroids; ++i )
	{
		asteroid = m_asteroids[ i ];
		if ( !asteroid->m_active )
			continue;

		asteroid->Tick( dt );

		sprite = m_spritesPool[ i ];
		sprite->m_position = asteroid->m_pos;

		if ( sprite->m_position.SqrMagnitude() > 9.0f )
			Deactivate( i );
	}
}

Bool AsteroidsManager::TryDestroyAsteroid( const Vector2& point )
{
	for ( Uint32 i = 0; i < m_maxAsteroids; ++i )
	{
		if ( m_asteroids[ i ]->Intersects( point ) )
		{
			Deactivate( i );
			return true;
		}
	}

	return false;
}

void AsteroidsManager::Spawn()
{
	Asteroid* asteroid = nullptr;
	Sprite* sprite = nullptr;
	for ( Uint32 i = 0; i < m_maxAsteroids; ++i )
	{
		if ( !m_asteroids[ i ]->m_active )
		{
			asteroid = m_asteroids[ i ];
			sprite = m_spritesPool[ i ];
			break;
		}
	}

	if ( !asteroid )
		return; // no free asteroid found

	Vector2 position = 1.5f * helper::RandomOnUnitCircle();

	asteroid->m_direction = 0.5f * helper::RandomInUnitCircle() - position;
	asteroid->m_direction.Normalize();

	asteroid->m_pos = position;
	asteroid->m_active = true;

	sprite->m_position = asteroid->m_pos;
}

void AsteroidsManager::Deactivate( Uint32 asteroidIndex )
{
	m_asteroids[ asteroidIndex ]->m_active = false;
	m_asteroids[ asteroidIndex ]->m_pos = Vector2( 1000, 1000 );
	m_spritesPool[ asteroidIndex ]->m_position = m_asteroids[ asteroidIndex ]->m_pos;
}
