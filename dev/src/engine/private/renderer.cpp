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

Renderer::Renderer()
	: m_sprites( nullptr )
{
}

Renderer::~Renderer()
{
	delete m_sprites;
	m_sprites = nullptr;
}

void Renderer::Init()
{
	graphics::GenerateVertexArraysObject();

	m_sprites = new SpritesRenderer;
	m_sprites->Init();

	graphics::SetClearColor( 0.5f, 0.5f, 0.5f, 1 );
}

void Renderer::Draw()
{
	graphics::ClearColorAndDepth();

	m_sprites->Draw();

	graphics::SwapBuffers();
}

Sprite* Renderer::CreateSprite() const
{
	return m_sprites->CreateSprite();
}
