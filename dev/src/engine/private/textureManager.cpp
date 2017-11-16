#include "textureManager.h"

#include "graphics.h"

#include "../../utils/public/image.h"
#include "../../utils/public/macros.h"
#include "../../utils/public/hash.h"
#include "../../utils/public/fileUtils.h"

void TextureManager::LoadTexture( const AnsiChar* name )
{
	InternalLoadTexture( sc::GetResourcePath( name ).c_str() );
}

Uint32 TextureManager::FindTexture( const AnsiChar* fileName )
{
	const Uint32 hash = sc::GetHash32( fileName );

	for ( Uint32 i = 0; i < m_textureHashes.size(); ++i )
	{
		if ( m_textureHashes[ i ] == hash )
		{
			return m_textureIds[ i ];
		}
	}

	return 0; // texture not found
}

void TextureManager::InternalLoadTexture( const AnsiChar* path )
{
	const AnsiChar* filename = &path[ sc::path::FileNameIndex( path ) ];
	const Uint32 hash = sc::GetHash32( filename );

	SC_VALIDATE(
		for ( Uint32 h : m_textureHashes )
		{
			SC_ASSERT( h != hash, "Hash collision occured" );
		}
	);

	auto img = Image::Load( path );
	SC_ASSERT( img, "Failed to load image" );

	const Uint32 textureId = graphics::CreateTexture2D();
	graphics::ActivateTextureUnit0();
	graphics::BindTexture2D( textureId );

	graphics::UploadTexture2D( textureId, img->GetWidth(), img->GetHeight(), img->GetData() );

	m_textureHashes.push_back( hash );
	m_textureIds.push_back( textureId );

	SC_ASSERT( m_textureHashes.size() == m_textureIds.size(), "" );

	delete img;
}