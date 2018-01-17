#include "missilesManager.h"

#include "../../engine/public/engine.h"
#include "../../engine/public/sprite.h"
#include "../../engine/public/textureManager.h"

#include <math.h>

extern Engine* GEngine;

class Missile
{
public:
	Missile()
		: m_ttl( 0.0f )
		, m_active( false )
		, m_position( 1000, 1000 )
	{ }

	Vector2 m_position;
	Vector2 m_direction;
	Float m_ttl;
	Bool m_active;
};


MissilesManager::MissilesManager()
	: m_timeSinceLastShot( 0.0f )
{
}

MissilesManager::~MissilesManager()
{
}

void MissilesManager::LoadResources( TextureManager* textures )
{
	const AnsiChar* textureName = "missile.png";
	textures->LoadTexture( textureName );
	const Uint32 texture = textures->FindTexture( textureName );

	const Uint32 maxMissiles = static_cast< Uint32 >( std::ceilf( c_maxMissiles ) );
	m_missiles.reserve( maxMissiles );
	m_spritesPool.reserve( maxMissiles );

	for ( Uint32 i = 0; i < maxMissiles; ++i )
	{
		Missile* missile = new Missile;
		Sprite* sprite = GEngine->CreateSprite();
		sprite->SetTexture( texture );
		sprite->m_position = missile->m_position;
		sprite->m_scale = { 0.03f, 0.03f };

		m_missiles.push_back( missile );
		m_spritesPool.push_back( sprite );
	}
}

void MissilesManager::Tick( Float dt )
{
	m_timeSinceLastShot += dt;
	for ( Uint32 i = 0; i < m_missiles.size(); ++i )
	{
		if ( m_missiles[ i ]->m_active )
		{
			if ( m_missiles[ i ]->m_ttl > c_missileLifetime )
			{
				m_missiles[ i ]->m_active = false;
				m_missiles[ i ]->m_position = Vector2( 1000, 1000 );
			}
			else
			{
				m_missiles[ i ]->m_position += m_missiles[ i ]->m_direction * dt * 4.0f;
			}

			m_spritesPool[ i ]->m_position = m_missiles[ i ]->m_position;
			m_missiles[ i ]->m_ttl += dt;
		}
	}
}

void MissilesManager::TryShoot( const Vector2& position, const Vector2& direction )
{
	if ( m_timeSinceLastShot > c_shootingInterval )
	{
		m_timeSinceLastShot = 0.0f;
		for ( Uint32 i = 0; i < m_missiles.size(); ++i )
		{
			if ( !m_missiles[ i ]->m_active )
			{
				Missile* missile = nullptr;
				missile = m_missiles[ i ];
				missile->m_active = true;
				missile->m_position = position;
				missile->m_direction = direction;
				missile->m_ttl = 0.0f;

				// calculate rotation
				static const Vector2 up( 0, 1 );
				m_spritesPool[ i ]->m_rotation = missile->m_direction.Angle( up );
				if ( missile->m_direction.x > 0.0f )
					m_spritesPool[ i ]->m_rotation *= -1.0f;

				break;
			}
		}
	}
}

void MissilesManager::CheckCollision( std::function<Bool( const Vector2& ) > collides )
{
	for ( Missile* missile : m_missiles )
	{
		if ( collides( missile->m_position ) )
		{
			missile->m_ttl = c_missileLifetime;
			break;
		}
	}
}
