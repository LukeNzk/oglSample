#include "graphics.h"

#pragma  comment( lib, "opengl32.lib" )

#include "../../utils/public/macros.h"
#include "window.h"

#include <windows.h>
#include <gl/GL.h>

namespace graphics 
{
	void ResizeViewport( Uint32 width, Uint32 height )
	{
		glViewport( 0, 0, width, height );
	}

	void Init( void* windowHandle )
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,             //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                       //Number of bits for the depthbuffer
			8,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC hDC = GetDC( reinterpret_cast< HWND >( windowHandle ) );
		Uint32 pixelFormatID = ChoosePixelFormat( hDC, &pfd );
		BOOL success = SetPixelFormat( hDC, pixelFormatID, &pfd );
		SC_ASSERT( success, "Failed to set pixel format." );

		HGLRC hglrc = wglCreateContext( hDC );

		success = wglMakeCurrent( hDC, hglrc );
		SC_ASSERT( success, "Failed to set current context." );
	}

	void LoadFunctions()
	{
		// load opengl functions
	}
}