#pragma once
#include "../../utils/public/types.h"
#include "../../utils/public/vector2.h"

class Movement
{
public:
	void SetMousePosition( Vector2 wsMousePos );
	void Move( Float dt );
	void SetThrusters( Bool enabled );

	Float	m_rotation;
	Vector2 m_position;

private:
	Vector2 m_direction;
	Vector2 m_targetDirection;
	Vector2 m_mousePosition;

	Float	m_desiredSpeed;
	Float	m_speed;
};

