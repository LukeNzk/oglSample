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
	Vector2 m_targetDirection;

private:
	Vector2 m_mousePosition;
	Vector2 m_direction;

	Float	m_desiredSpeed;
	Float	m_speed;
};

