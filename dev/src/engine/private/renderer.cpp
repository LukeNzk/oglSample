#include "renderer.h"
#include "../../utils/public/types.h"
#include "../../utils/public/absolutePath.h"
#include "../../utils/public/fileUtils.h"
#include "../../utils/public/matrix.h"
#include "../../utils/public/image.h"

#include "graphics.h"
#include "spritesRenderer.h"
#include "sprite.h"

#include <stdio.h>
#include <iostream>
#include <memory>

SpritesRenderer gSpriteRenderer;

void Renderer::Init()
{
	graphics::GenerateVertexArraysObject();

	gSpriteRenderer.Init();
	Sprite* spr = gSpriteRenderer.CreateSprite();

	spr->m_position = Vector2( 0, 0.5f );
	spr->m_scale = 0.2f;
	spr->m_rotation = 0.2f;

	graphics::SetClearColor( 0.5f, 0.5f, 0.5f, 1 );
}

void Renderer::Draw()
{
	graphics::ClearColorAndDepth();

	gSpriteRenderer.Draw();

	graphics::SwapBuffers();
}
