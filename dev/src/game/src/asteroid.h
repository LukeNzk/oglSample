#pragma once
#include "../../utils/public/vector2.h"

class Asteroid
{
public:
	Asteroid();
	~Asteroid();

	Bool Intersects( Vector2 point, Float radius = 0.0f ) const;
	void Tick( Float dt );
	void Destroy();

	Vector2 m_pos;
	Vector2 m_direction;
	Float	m_radius;
	Bool	m_active;
};