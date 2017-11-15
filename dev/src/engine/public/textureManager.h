#pragma once
#include "../../utils/public/types.h"

#include <vector>

class TextureManager
{
public:
	void LoadTextures();

	Uint32 FindTexture( const AnsiChar* path );

private:
	void LoadTexture( const AnsiChar* path );

private:
	std::vector< Uint32 > m_textureHashes;
	std::vector< Uint32 > m_textureIds;
};