#pragma once
#include "../../utils/public/types.h"

#include <memory>

class SpritesRenderer;
class Sprite;
class TextureManager;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();
	void Draw();

	Sprite* CreateSprite() const;

private:
	SpritesRenderer*	m_sprites;
};