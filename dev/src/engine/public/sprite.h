#pragma once
#include "../../utils/public/types.h"
#include "../../utils/public/vector2.h"

class Sprite
{
public:
	Sprite();

	void SetTexture( Uint32 textureId )
	{
		m_texture = textureId;
	}

	Uint32 GetTexture() const { return m_texture; }

public:
	Vector2 m_position;
	Vector2	m_scale;
	Float	m_rotation;

private:
	Uint32 m_texture;
};