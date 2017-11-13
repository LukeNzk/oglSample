#include "renderer.h"
#include "../../utils/public/types.h"
#include "graphics.h"

#include <stdio.h>
#include <iostream>

Uint32 bufferId;
Uint32 gpuProg;

void Renderer::Init()
{
	graphics::GenerateVertexArraysObject();

	float verts[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};

	bufferId = graphics::CreateArrayBuffer();
	graphics::BindArrayBuffer( bufferId );
	graphics::LoadStaticBufferData( bufferId, 9 * sizeof( Float ), verts );

	Uint32 vShader = graphics::LoadShader( "D:\\vertex.hlsl", true );
	Uint32 fShader = graphics::LoadShader( "D:\\fragm.hlsl", false );
	gpuProg = graphics::CreateProgram( vShader, fShader );

	graphics::DeleteShader( vShader );
	graphics::DeleteShader( fShader );
	graphics::SetClearColor( 0.5f, 0.5f, 0.5f, 1 );
}

void Renderer::Draw()
{
	graphics::ClearColorAndDepth();

	graphics::UseProgram( gpuProg );
	graphics::DrawVertexBuffer( bufferId );

	graphics::SwapBuffers();
}
