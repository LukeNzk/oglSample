#include "game.h"

#include "..\..\engine\public\engine.h"
#include "..\..\engine\public\textureManager.h"
#include "..\..\engine\public\sprite.h"
#include "..\..\engine\public\input.h"

#include "../../utils/public/vector2.h"
#include "../../utils/public/debug.h"
#include "../../utils/public/timer.h"

#include "imageBuffer.h"
#include "tgaBuffer.h"
#include "triangle.h"
#include "shader.h"
#include "model.h"

extern Engine* GEngine;

Shader shader;

Game::Game()
	: m_renderTarget( new TgaBuffer( 800, 600 ) )
{
	Model* model = new Model();
	model->Load( "D:/cube.txt" );
	model->SetPosition( { 1, 2.0f, -2.f } );
	m_models.push_back( model );

	model = new Model();
	model->Load( "D:/sphere.txt" );
	model->SetPosition( { 0, 0.0f, -4.f } );
	m_models.push_back( model );

	model = new Model();
	model->Load( "D:/cube.txt" );
	model->SetPosition( { 2, -1.0f, -1.f } );
	m_models.push_back( model );

	model = new Model();
	model->Load( "D:/cube.txt" );
	model->SetPosition( { 4, 1.0f, -3.f } );
	m_models.push_back( model );

	model = new Model();
	model->Load( "D:/cube.txt" );
	model->SetPosition( { -2, 1.0f, -3.f } );
	m_models.push_back( model );
}

Game::~Game()
{
	delete m_renderTarget;

	for ( auto model : m_models )
		delete model;

	m_models.clear();
}

void Game::LoadResources()
{
	const Uint32 data[ 4 ] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff00 };
	GEngine->SetOverlayData( 2, 2, data );
}

void Game::Tick( Float dt )
{
	static Timer timer( true );
	static Uint32 frame = 0;

	m_renderTarget->Clear( 0xff333333 );

	float4x4 proj;
	proj.SetProjection( 65.f, m_renderTarget->GetAspectRatio(), 0.1f, 1000.f );

	float4x4 view;
	view.LookAt( { 0.f, 0.f, -9.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f } );

	shader.SetViewProjectionMatrix( view, proj );

	for ( auto model : m_models )
		model->Draw( m_renderTarget, &shader );

	GEngine->SetOverlayData( m_renderTarget->Width(), m_renderTarget->Height(),
							 m_renderTarget->GetColorData() );

	++frame;

	if ( frame % 100 == 0 )
	{
		const Double fps = ( Double )frame / timer.TimeElapsed();
		std::printf( "FPS: %f\n", fps );
		frame = 0;
		timer.Start();
	}
}

void Game::DispatchEvent( ERIEventType type, void* data )
{
}