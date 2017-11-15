#include "image.h"
#include "macros.h"
#include "../../../external/freeimage/x64/FreeImage.h"

#pragma comment( lib, "x64/FreeImage.lib")
#include <memory>

namespace helper
{
	FIBITMAP* LoadImage( const AnsiChar* path, Int32 flag = 0 )
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType( path );
		if (fif == FIF_UNKNOWN)
		{
			fif = FreeImage_GetFIFFromFilename( path );
		}
		
		SC_ASSERT( fif != FIF_UNKNOWN, "Unknown image format: [%s].", path );
		SC_ASSERT( FreeImage_FIFSupportsReading( fif ), "Image format not supported." );
		FIBITMAP* dib = FreeImage_Load( fif, path, flag );
		return dib;
	}
}

Image::Image()
	: m_data( nullptr )
{
}

Image::~Image()
{
	delete[] m_data;
	m_data = nullptr;
}

Image* Image::Load( const AnsiChar* path )
{
	Image* result = new Image;
	
	FIBITMAP* bmp = helper::LoadImage( path, 0 );
	const Uint32 bitsPerPixel = FreeImage_GetBPP( bmp );
	SC_ASSERT( bitsPerPixel == 4 * 8, "Unsupported image bits per pixel [%d].", bitsPerPixel );

	Uint32 w = FreeImage_GetWidth( bmp );
	Uint32 h = FreeImage_GetHeight( bmp );
	Uint8* data = new Uint8[ w * h * 4 ];
	memcpy( data, FreeImage_GetBits( bmp ), w * h * 4 );
	delete bmp;
	return result;
}