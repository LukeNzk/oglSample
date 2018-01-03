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
	extern void UploadStaticBufferData( Uint32 buffer, Uint32 n, const Float* data );
	extern void DrawTriangles( Uint32 n );
	extern void DrawTriangleStrip( Uint32 n );
	extern void EnableSpriteAttributes();
	extern void DisableSpriteAttributes();
	extern Uint32 GenerateVertexArraysObject();

	extern Uint32 CreateTexture2D();
	extern void BindTexture2D( Uint32 texture );
	extern void UploadOverlayTexture( Uint32 texture, Uint32 width, Uint32 height, const void* data );
	extern void UploadTexture2D( Uint32 texture, Uint32 width, Uint32 height, const void* data );
	extern void ActivateTextureUnit0();

	extern Uint32 LoadShader( const AnsiChar* path, Bool vertex );
	extern Uint32 CreateProgram( Uint32 vertexShader, Uint32 fragmentShader );
	extern void UseProgram( Uint32 program );
	extern void DeleteShader( Uint32 shader );

	extern Uint32 GetUniformLocation( Uint32 program, const AnsiChar* name );
	extern void SetUniform1i( Uint32 location, Int32 value );
	extern void SetUniform1f( Uint32 location, Float value );
	extern void SetUniform2f( Uint32 location, const Float* value );
	extern void SetUniform3f( Uint32 location, const Float* value );
	extern void SetUniformMatrix( Uint32 location, const Float* matrix );
}