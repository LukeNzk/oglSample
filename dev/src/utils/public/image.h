#pragma once
#include "types.h"

class Image
{
public:
	~Image();

	static Image* Load( const AnsiChar* path );
	const void* GetData() const { return m_data; }

	Uint32 GetWidth() const { return m_width; }
	Uint32 GetHeight() const { return m_height; }

private:
	Image();

private:
	void* m_data;

	Uint32 m_width;
	Uint32 m_height;
};