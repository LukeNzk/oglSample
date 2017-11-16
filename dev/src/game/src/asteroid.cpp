#include "asteroid.h"

Asteroid::Asteroid( Float radius )
	: m_radius( radius )
{
}

Asteroid::~Asteroid()
{
}

Bool Asteroid::Intersects( Vector2 point ) const
{
	Vector2 asteroidToPoint = point - m_pos;
	if ( asteroidToPoint.SqrMagnitude() < m_radius * m_radius )
		return true;

	return false;
}
