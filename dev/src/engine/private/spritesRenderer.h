#pragma once
#include "../../utils/public/types.h"

#include <vector>
#include <memory>

class Sprite;
class SpriteShader;

class SpritesRenderer
{
public:
	SpritesRenderer();
	~SpritesRenderer();

	void Init();
	void Draw();

	Sprite* CreateSprite();

private:
	std::vector< Sprite* >	m_sprites;

	typedef std::unique_ptr< SpriteShader > SpriteShaderUPtr;
	SpriteShaderUPtr		m_shader;
	Uint32					m_vbo;
};