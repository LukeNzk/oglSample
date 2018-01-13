#include "tgaBuffer.h"
#include <iostream>
#include <fstream>
#include <memory.h>

namespace helper
{
	void WriteTGAHeader(std::fstream& fs, int width, int height)
	{
#pragma pack( 1 )
		typedef struct {
			char  idlength;
			char  colourmaptype;
			char  datatypecode;
			short int colourmaporigin;
			short int colourmaplength;
			char  colourmapdepth;
			short int x_origin;
			short int y_origin;
			short width;
			short height;
			char  bitsperpixel;
			char  imagedescriptor;
		} HEADER;

		HEADER h;

		std::memset(&h, 0, sizeof(HEADER));
		h.datatypecode = 2;
		h.width = width;
		h.height = height;
		h.bitsperpixel = 32;

		const char* dataPtr = reinterpret_cast< const char* >( &h );
		fs.write(dataPtr, sizeof(HEADER));
	}
}

TgaBuffer::TgaBuffer(Uint32 width, Uint32 height)
	: m_color( nullptr )
	, m_depth( nullptr )
{
	SetSize( width, height );
	m_color = new Uint32[ m_height * m_width ] { 0u };
	m_depth = new Float[ m_height * m_width ] { std::numeric_limits< float >::max() };
}

TgaBuffer::~TgaBuffer()
{
	delete m_color;
	m_color = nullptr;

	delete m_depth;
	m_depth = nullptr;
}

void TgaBuffer::WriteColor(Uint32 x, Uint32 y, Uint32 color)
{
	m_color[x + y * m_width] = color;
}

void TgaBuffer::WriteColor(Uint32 i, Uint32 color)
{
	m_color[i] = color;
}

void TgaBuffer::WriteDepth( Uint32 x, Uint32 y, Float depth )
{
	m_depth[ x + y * m_width ] = depth;
}

void TgaBuffer::ClearDepth()
{
	const Uint32 n = Size();
	for ( Uint32 i = 0; i < n; ++i )
	{
		m_depth[ i ] = std::numeric_limits< float >::max();
	}
}

void TgaBuffer::Clear( Uint32 color )
{
	const Uint32 n = Size();
	for ( Uint32 i = 0; i < n; ++i )
	{
		m_depth[ i ] = std::numeric_limits< float >::max();
		m_color[ i ] = color;
	}
}

void TgaBuffer::Save(const char * fileName)
{
	std::fstream fs;
	
	fs.open(fileName, std::fstream::out | std::fstream::binary);
	helper::WriteTGAHeader(fs, Width(), Height());
	fs.write(static_cast<const char*>((void*)m_color), DataSize());
	fs.close();
}
