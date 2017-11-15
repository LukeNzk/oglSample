#include "graphics.h"
#pragma  comment( lib, "opengl32.lib" )

#include "glExtenstions.h"

#include "../../utils/public/macros.h"
#include "../../utils/public/types.h"
#include "../../utils/public/fileStream.h"
#include "../../utils/public/fileUtils.h"
#include "../../utils/public/matrix.h"

#include "window.h"

#include <string.h>
#include <memory>

#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE0                       0x84C0
#define GL_BGRA                           0x80E1
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

namespace graphics
{
	HDC gDeviceContext = NULL;
	HGLRC gRenderContext = NULL;
	HWND gMainHWND = NULL;

	void ResizeViewport( Uint32 width, Uint32 height )
	{
		glViewport( 0, 0, width, height );
	}

	void InitExtensions()
	{
		WNDCLASSEX wc;
		HINSTANCE hInstance = GetModuleHandle( NULL );

		const AnsiChar* className = "oglInit";
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
		wc.lpszMenuName = NULL;
		wc.lpszClassName = className;
		wc.cbSize = sizeof( WNDCLASSEX );

		RegisterClassEx( &wc );

		HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW, className, className, WS_POPUP,
								 0, 0, 100, 100, NULL, NULL, hInstance, NULL );

		ShowWindow( hWnd, SW_HIDE );

		const Bool initExtensions = glInternal::InitExtensions( hWnd );
		SC_ASSERT( initExtensions, "Failed to init extensions" );

		DestroyWindow( hWnd );
	}

	void Init( void* windowHandle )
	{
		SC_ASSERT( gMainHWND == NULL, "Graphics already initialized." );
		gMainHWND = reinterpret_cast< HWND >( windowHandle );

		gDeviceContext = GetDC( gMainHWND );
		SC_ASSERT( gDeviceContext, "Failed to create device context." );

		InitExtensions();

		Int32 pixelFormatArb[ 19 ] =
		{
			WGL_SUPPORT_OPENGL_ARB, TRUE,
			WGL_DRAW_TO_WINDOW_ARB, TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 24,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_DOUBLE_BUFFER_ARB, TRUE,
			WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		Int32 noErr;
		Int32 pixelFormat;
		Uint32 formatCount;
		noErr = wglChoosePixelFormatARB( gDeviceContext, pixelFormatArb, NULL, 1, &pixelFormat, &formatCount );
		SC_ASSERT( noErr, "Could not finde pixel format." );

		PIXELFORMATDESCRIPTOR pfd;

		noErr = SetPixelFormat( gDeviceContext, pixelFormat, &pfd );
		SC_ASSERT( noErr, "Could not set pixel format." );

		Int32 glAttributes[ 5 ] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0
		};

		gRenderContext = wglCreateContextAttribsARB( gDeviceContext, 0, glAttributes );
		SC_ASSERT( gRenderContext, "Failed to create render context." );

		noErr = wglMakeCurrent( gDeviceContext, gRenderContext );
		SC_ASSERT( noErr, "Could not set device context." );

		glClearDepth( 1.0f );

		glEnable( GL_DEPTH_TEST );
		glFrontFace( GL_CW );

		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );

		glEnable( GL_BLEND ); 
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		const Bool vsync = true;
		if ( vsync )
			noErr = wglSwapIntervalEXT( 1 );
		else
			noErr = wglSwapIntervalEXT( 0 );

		SC_ASSERT( noErr, "Failed to set vsync" );
	}

	void Shutdown()
	{
		if ( gRenderContext )
		{
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( gRenderContext );
			gRenderContext = NULL;
		}

		if ( gDeviceContext )
		{
			ReleaseDC( gMainHWND, gDeviceContext );
			gDeviceContext = NULL;
		}
	}

	void ClearColorAndDepth()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		SC_ASSERT( glGetError() == 0, "Failed to clear color or depth buffer." );
	}

	void SetClearColor( Float r, Float g, Float b, Float a )
	{
		glClearColor( r, g, b, a );
		SC_ASSERT( glGetError() == 0, "Failed to set clear color." );
	}
	
	void SwapBuffers()
	{
		SwapBuffers( gDeviceContext );
		SC_ASSERT( glGetError() == 0, "Failed to swap buffers." );
	}

	//////////////////////////////////////////////////////////////////////////
	// Array buffer
	//////////////////////////////////////////////////////////////////////////
	Uint32 CreateArrayBuffer()
	{
		GLuint buffer = 0;
		glGenBuffers( 1, &buffer );
		SC_ASSERT( glGetError() == 0, "Failed to generate array buffer." );
		return buffer;
	}

	void BindArrayBuffer( Uint32 buffer )
	{
		glBindBuffer( GL_ARRAY_BUFFER, buffer );
		SC_ASSERT( glGetError() == 0, "Failed binding array buffer." );
	}

	void UploadStaticBufferData( Uint32 buffer, Uint32 n, const Float* data )
	{
		BindArrayBuffer( buffer );
		glBufferData( GL_ARRAY_BUFFER, n, data, GL_STATIC_DRAW );
		SC_ASSERT( glGetError() == 0, "Failed setting array buffer data." );
	}

	Uint32 GenerateVertexArraysObject()
	{
		GLuint vao = 0;

		Uint32 error = 0;
		glGenVertexArrays( 1, &vao );
		SC_ASSERT( glGetError() == 0, "Failed to generate VAO." );

		glBindVertexArray( vao );
		SC_ASSERT( glGetError() == 0, "Failed to bind VAO." );

		return vao;
	}

	//////////////////////////////////////////////////////////////////////////
	// Texture
	//////////////////////////////////////////////////////////////////////////
	Uint32 CreateTexture2D()
	{
		GLuint texture = 0;
		glGenTextures( 1, &texture );
		SC_ASSERT( glGetError() == 0, "Failed to generate texture." );
		return texture;
	}

	void BindTexture2D( Uint32 texture )
	{
		glBindTexture( GL_TEXTURE_2D, texture );
		SC_ASSERT( glGetError() == 0, "Failed to bind 2D texture." );
	}

	void UploadTexture2D( Uint32 texture, Uint32 width, Uint32 height, const void* data )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data );
		SC_ASSERT( glGetError() == 0, "Failed to bind 2D texture." );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

		glGenerateMipmap(GL_TEXTURE_2D);
		SC_ASSERT( glGetError() == 0, "Failed to set mipmaps for 2D texture." );
	}

	void ActivateTextureUnit0()
	{
		glActiveTexture( GL_TEXTURE0 );
		SC_ASSERT( glGetError() == 0, "Failed to activate texture unit." );
	}

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	//////////////////////////////////////////////////////////////////////////
	void DrawTriangles( Uint32 n )
	{
		glEnableVertexAttribArray( 0 );
		SC_ASSERT( glGetError() == 0, "Failed to enable vertex attrib array." );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, ( void* )0 );
		SC_ASSERT( glGetError() == 0, "Failed to initialize vertex attrib pointer." );

		glDrawArrays( GL_TRIANGLES, 0, 3 * n );
		SC_ASSERT( glGetError() == 0, "Failed to draw arrays." );

		glDisableVertexAttribArray( 0 );
		SC_ASSERT( glGetError() == 0, "Failed to disable vertex attrib pointer." );
	}

	void DrawTriangleStrip( Uint32 n )
	{
		glEnableVertexAttribArray( 0 );
		SC_ASSERT( glGetError() == 0, "Failed to enable vertex attrib array." );

		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( Float ), ( void* )0 );
		SC_ASSERT( glGetError() == 0, "Failed to initialize 1st vertex attrib pointer." );

		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( Float ), ( void* )( 2 * sizeof( Float ) ) );
		SC_ASSERT( glGetError() == 0, "Failed to initialize 2nd vertex attrib pointer." );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, n );
		SC_ASSERT( glGetError() == 0, "Failed to draw arrays." );

		glDisableVertexAttribArray( 0 );
		SC_ASSERT( glGetError() == 0, "Failed to disable vertex attrib pointer." );

		glDisableVertexAttribArray( 1 );
		SC_ASSERT( glGetError() == 0, "Failed to disable vertex attrib pointer." );
	}

	//////////////////////////////////////////////////////////////////////////
	// Shaders
	//////////////////////////////////////////////////////////////////////////
	Uint32 LoadShader( const AnsiChar* path, Bool vertex )
	{
		std::unique_ptr< sc::FileStream > fs( sc::FileStream::Open( path ) );
		const Uint32 size = fs->GetSize();
		AnsiChar* code = new AnsiChar[ size + 1 ];
		fs->Read( code, size );
		code[ size ] = 0;

		// create shader
		const Uint32 shaderType = ( vertex ) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		const GLuint shaderId = glCreateShader( shaderType );

		// compile shader
		const Int8* codePtr = reinterpret_cast< Int8* >( code );
		glShaderSource( shaderId, 1, &codePtr, NULL );
		glCompileShader( shaderId );

		Int32 noErr = 0;
		glGetShaderiv( shaderId, GL_COMPILE_STATUS, &noErr );
		
		Int32 logLength;
		glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &logLength );
		if ( logLength > 0 )
		{
			Int8* log = new Int8[ logLength ];
			glGetShaderInfoLog( shaderId, logLength, NULL, log );
			SC_ASSERT( false, log );

			delete[] log;
		}

		delete[] code;

		return shaderId;
	}

	Uint32 CreateProgram( Uint32 vertexShader, Uint32 fragmentShader )
	{
		Uint32 program = glCreateProgram();
		glAttachShader( program, vertexShader );
		glAttachShader( program, fragmentShader );

		glLinkProgram( program );

		// validate shader
		Int32 noError;
		Int32 logLength;
		glGetProgramiv( program, GL_LINK_STATUS, &noError );
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		if ( logLength > 0 ) {
			Int8* log = new Int8[ logLength ];
			glGetShaderInfoLog( program, logLength, NULL, log );
			SC_ASSERT( false, log );

			delete[] log;
		}

		glDetachShader( program, vertexShader );
		glDetachShader( program, fragmentShader );

		return program;
	}


	void UseProgram( Uint32 program )
	{
		glUseProgram( program );
	}

	void DeleteShader( Uint32 shader )
	{
		glDeleteShader( shader );
	}

	Uint32 GetUniformLocation( Uint32 program, const AnsiChar* name )
	{
		const Uint32 result = glGetUniformLocation( program, name );
		SC_ASSERT( glGetError() == 0, "Failed to get unifrom location [%s].", name );
		SC_ASSERT( result != 0xffffffff, "Failed to get unifrom location [%s].", name );
		return result;
	}

	void SetUniform1i( Uint32 location, Int32 value )
	{
		glUniform1i( location, value );
		SC_ASSERT( glGetError() == 0, "Failed to set integer unifrom." );
	}

	void SetUniform1f( Uint32 location, Float value )
	{
		glUniform1f( location, value );
		SC_ASSERT( glGetError() == 0, "Failed to set float unifrom." );
	}

	void SetUniform2f( Uint32 location, const Float * value )
	{
		glUniform2fv( location, 1, value );
		SC_ASSERT( glGetError() == 0, "Failed to set float3 unifrom." );
	}

	void SetUniform3f( Uint32 location, const Float* value )
	{
		glUniform3fv( location, 1, value );
		SC_ASSERT( glGetError() == 0, "Failed to set float3 unifrom." );
	}

	void SetUniformMatrix( Uint32 location, const Float* matrix )
	{
		glUniformMatrix4fv( location, 1, GL_FALSE, matrix );
		SC_ASSERT( glGetError() == 0, "Failed to set matrix unifrom." );
	}
}