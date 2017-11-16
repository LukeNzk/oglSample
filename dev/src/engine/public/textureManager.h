#pragma once
#include "../../utils/public/types.h"

#include <vector>

class TextureManager
{
public:
	void LoadTexture( const AnsiChar* path );

	Uint32 FindTexture( const AnsiChar* path );

private:
	void InternalLoadTexture( const AnsiChar * path );

private:
	std::vector< Uint32 > m_textureHashes;
	std::vector< Uint32 > m_textureIds;
};