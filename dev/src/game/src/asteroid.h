#pragma once
#include "../../utils/public/vector2.h"

class Asteroid
{
public:
	Asteroid();
	~Asteroid();

	Bool Intersects( Vector2 point ) const;
	void Tick( Float dt );
	void Destroy();

	Vector2 m_pos;
	Float	m_rotation;
	Float	m_radius;
	Bool	m_active;
};