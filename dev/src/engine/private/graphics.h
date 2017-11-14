#pragma once
#include "../../utils/public/types.h"

namespace graphics
{
	extern void Init( void* windowHandle );
	extern void ResizeViewport( Uint32 width, Uint32 height );

	extern void ClearColorAndDepth();
	extern void SetClearColor( Float r, Float g, Float b, Float a );
	extern void SwapBuffers();

	extern Uint32 CreateArrayBuffer();
	extern void BindArrayBuffer( Uint32 gid );
	extern void LoadStaticBufferData( Uint32 buffer, Uint32 n, const Float* data );
	extern void DrawTriangles( Uint32 buffer, Uint32 n );
	extern void DrawTriangleStrip( Uint32 buffer, Uint32 n );
	extern Uint32 GenerateVertexArraysObject();

	extern Uint32 LoadShader( const AnsiChar* path, Bool vertex );
	extern Uint32 CreateProgram( Uint32 vertexShader, Uint32 fragmentShader );
	extern void UseProgram( Uint32 program );
	extern void DeleteShader( Uint32 shader );
	extern void SetUniform1f( Uint32 location, Float value );
}