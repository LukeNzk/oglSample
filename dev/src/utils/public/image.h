#pragma once
#include "types.h"

class Image
{
public:
	~Image();

	static Image* Load( const AnsiChar* path );
	const void* GetData() const { return m_data; }

private:
	Image();

private:
	void* m_data;
};