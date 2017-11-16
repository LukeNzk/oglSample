#pragma once
#include "../../utils/public/vector2.h"

class Asteroid
{
public:
	Asteroid( Float radius );
	~Asteroid();

	Bool Intersects( Vector2 point ) const;
	
	Vector2 m_pos;
	Float	m_rotation;
	Float	m_radius;
};