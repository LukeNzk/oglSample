#include "asteroid.h"

Asteroid::Asteroid()
	: m_radius( 0 )
	, m_pos( 1000, 1000 )
	, m_active( false )
{
}

Asteroid::~Asteroid()
{
}

Bool Asteroid::Intersects( Vector2 point, Float radius ) const
{
	Vector2 asteroidToPoint = point - m_pos;
	if ( asteroidToPoint.SqrMagnitude() < ( m_radius * m_radius + radius * radius ) )
		return true;

	return false;
}

void Asteroid::Tick( Float dt )
{
	m_pos += m_direction * dt;
}

void Asteroid::Destroy()
{
	m_active = false;
	m_pos = Vector2( 1000, 1000 );
}
