#include "movement.h"

namespace helper
{
	Vector2 SSToWS( Vector2& pos, Uint32 w, Uint32 h )
	{
		const Vector2 center( w * 0.5f, h * 0.5f );

		Vector2 result;
		result = 2.0f * ( pos - center );
		result.y = -result.y;
		result.x /= w;
		result.y /= h;

		return result;
	}

	Float Lerp( Float a, Float b, Float t )
	{
		return a + ( b - a ) * t;
	}
}

void Movement::SetMousePosition( Vector2 wsMousePos )
{
	m_mousePosition = helper::SSToWS( wsMousePos, 800, 600 );
}

void Movement::Move( Float dt )
{
	static Vector2 up( 0, 1 );

	m_targetDirection = m_mousePosition - m_position;
	m_speed = helper::Lerp( m_speed, m_desiredSpeed, 0.2f );

	if ( m_targetDirection.SqrMagnitude() < 0.001f )
	{
		m_targetDirection = Vector2( 0, 0 );
	}
	else
	{
		m_targetDirection.Normalize();
		m_rotation = up.Angle( m_targetDirection );
		if ( m_targetDirection.x > 0.0f )
			m_rotation = -m_rotation;
	}

	m_direction.Lerp( m_targetDirection, 0.05f );
	m_position += m_direction * m_speed * dt;
}

void Movement::SetThrusters( Bool enabled )
{
	m_desiredSpeed = enabled ? 1.0f : 0.0f;
}
