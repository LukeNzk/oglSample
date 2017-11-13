#pragma once
#include "../../utils/public/types.h"

namespace graphics
{
	extern void Init( void* windowHandle );
	extern void ResizeViewport( Uint32 width, Uint32 height );

	extern void ClearColorAndDepth();
	extern void SetClearColor( Float r, Float g, Float b, Float a );
	extern void SwapBuffers();
}