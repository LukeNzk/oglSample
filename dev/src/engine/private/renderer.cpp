#include "renderer.h"
#include "../../utils/public/types.h"
#include "../../utils/public/absolutePath.h"
#include "../../utils/public/fileUtils.h"

#include "graphics.h"

#include <stdio.h>
#include <iostream>

Uint32 bufferId;
Uint32 gpuProg;

void Renderer::Init()
{
	graphics::GenerateVertexArraysObject();

	float verts[] = {
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f
	};

	bufferId = graphics::CreateArrayBuffer();
	graphics::BindArrayBuffer( bufferId );
	graphics::LoadStaticBufferData( bufferId, 8 * sizeof( Float ), verts );

	const sc::AbsolutePath shadersDir( sc::GetResourcesDir().c_str() );

	sc::AbsolutePath vShaderFile( shadersDir );
	vShaderFile.SetFile( "vertex.glsl" );

	sc::AbsolutePath fShaderFile( shadersDir );
	fShaderFile.SetFile( "fragm.glsl" );

	Uint32 vShader = graphics::LoadShader( vShaderFile.Get(), true );
	Uint32 fShader = graphics::LoadShader( fShaderFile.Get(), false );

	gpuProg = graphics::CreateProgram( vShader, fShader );

	graphics::DeleteShader( vShader );
	graphics::DeleteShader( fShader );
	graphics::SetClearColor( 0.5f, 0.5f, 0.5f, 1 );
}

void Renderer::Draw()
{
	graphics::ClearColorAndDepth();

	graphics::UseProgram( gpuProg );
	graphics::DrawTriangleStrip( bufferId, 8 );

	graphics::SwapBuffers();
}
