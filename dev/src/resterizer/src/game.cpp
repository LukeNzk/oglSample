#include "game.h"

#include "..\..\engine\public\engine.h"
#include "..\..\engine\public\textureManager.h"
#include "..\..\engine\public\sprite.h"
#include "..\..\engine\public\input.h"

#include "../../utils/public/vector2.h"
#include "../../utils/public/debug.h"

#include "imageBuffer.h"
#include "tgaBuffer.h"
#include "triangle.h"
#include "shader.h"
#include "model.h"

extern Engine* GEngine;

CTriangle testTriangle;
Model* testModel;

Shader shader;

Game::Game()
	: m_renderTarget( new TgaBuffer( 600, 600 ) )
{
	testModel = Model::CreateCube( float4( 0.0f, 0.0f, 0.f ), float4( 0.2f, 0.2f, 0.2f ) );

	testTriangle.SetVertexPos( 0, float4( 0, 0, 0.0 ) );
	testTriangle.SetVertexColor( 0, Color( 0xff, 0, 0 ) );

	testTriangle.SetVertexPos( 1, float4( 1.f, 0.f, 0.f ) );
	testTriangle.SetVertexColor( 1, Color( 0, 0xff, 0 ) );

	testTriangle.SetVertexPos( 2, float4( 0, 1.0f, 0.f ) );
	testTriangle.SetVertexColor( 2, Color( 0, 0, 0xff ) );
}

Game::~Game()
{
	delete m_renderTarget;
	delete testModel;
}

void Game::LoadResources()
{
	const Uint32 data[ 4 ] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff00 };
	GEngine->SetOverlayData( 2, 2, data );
}

void Game::Tick( Float dt )
{
	m_renderTarget->Clear( 0xff333333 );
	testModel->Draw( m_renderTarget, &shader );

	GEngine->SetOverlayData( m_renderTarget->Width(), m_renderTarget->Height(),
							 m_renderTarget->GetColorData() );
}

void Game::DispatchEvent( ERIEventType type, void* data )
{
}