#include "spritesRenderer.h"
#include "sprite.h"
#include "graphics.h"

#include "../../utils/public/absolutePath.h"
#include "../../utils/public/fileUtils.h"
#include "../../utils/public/matrix.h"
#include "../../utils/public/vector2.h"
#include "../../utils/public/types.h"

namespace helper
{
	Uint32 LoadShaders( const AnsiChar* vertex, const AnsiChar* framgent )
	{
		const sc::AbsolutePath shadersDir( sc::GetResourcesDir().c_str() );

		sc::AbsolutePath vShaderFile( shadersDir );
		vShaderFile.SetFile( vertex );

		sc::AbsolutePath fShaderFile( shadersDir );
		fShaderFile.SetFile( framgent );

		const Uint32 vShader = graphics::LoadShader( vShaderFile.Get(), true );
		const Uint32 fShader = graphics::LoadShader( fShaderFile.Get(), false );

		const Uint32 program = graphics::CreateProgram( vShader, fShader );

		graphics::DeleteShader( vShader );
		graphics::DeleteShader( fShader );

		return program;
	}
}

//////////////////////////////////////////////////////////////////////////
// SpriteShader
//////////////////////////////////////////////////////////////////////////
class SpriteShader
{
public:
	SpriteShader()
	{
	}

	~SpriteShader()
	{
	}

	void Load()
	{
		m_programId = helper::LoadShaders( "vertex.glsl", "fragm.glsl" );
		Use();

		m_projection = graphics::GetUniformLocation( m_programId, "proj" );
		m_rotation = graphics::GetUniformLocation( m_programId, "rot" );
		m_scale = graphics::GetUniformLocation( m_programId, "scale" );
		m_position = graphics::GetUniformLocation( m_programId, "position" );
	}

	void SetProjectionMatrix( const Float* mat ) const
	{
		graphics::SetUniformMatrix( m_projection, mat );
	}

	void SetRotation( Float rot ) const
	{
		graphics::SetUniform1f( m_rotation, rot );
	}

	void SetScale( Float scale ) const
	{
		graphics::SetUniform1f( m_scale, scale );
	}

	void SetPosition( const Vector2& pos ) const
	{
		graphics::SetUniform2f( m_position, pos.GetPtr() );
	}

	void Use() const
	{
		graphics::UseProgram( m_programId );
	}

private:
	Uint32 m_programId;
	Uint32 m_projection;

	Uint32 m_position;
	Uint32 m_rotation;
	Uint32 m_scale;
};


//////////////////////////////////////////////////////////////////////////
// SpritesRenderer
//////////////////////////////////////////////////////////////////////////
SpritesRenderer::SpritesRenderer()
{
}

SpritesRenderer::~SpritesRenderer()
{
	for ( Sprite* sprite : m_sprites )
	{
		delete sprite;
		sprite = nullptr;
	}

	m_sprites.clear();
	m_sprites.swap( std::vector< Sprite* >() );
}

void SpritesRenderer::Init()
{
	// square
	const Float verts[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	m_vbo = graphics::CreateArrayBuffer();
	graphics::BindArrayBuffer( m_vbo );
	graphics::LoadStaticBufferData( m_vbo, 8 * sizeof( Float ), verts );
	
	m_shader.reset( new SpriteShader );
	m_shader->Load();

	const Matrix projection = Matrix::CreateOrthoProj( -1.33f, -1.0, 1.33f, 1.0f, 0.1f, 100.0f );
	m_shader->SetProjectionMatrix( projection.GetData() );
}

void SpritesRenderer::Draw()
{
	m_shader->Use();
	graphics::BindArrayBuffer( m_vbo );

	for ( Sprite* sprite : m_sprites )
	{
		if ( sprite == nullptr )
			continue;

		m_shader->SetRotation( sprite->m_rotation );
		m_shader->SetScale( sprite->m_scale );
		m_shader->SetPosition( sprite->m_position );

		graphics::DrawTriangleStrip( m_vbo, 8 );
	}
}

Sprite* SpritesRenderer::CreateSprite()
{
	Sprite* spr = new Sprite();
	m_sprites.push_back( spr );
	return spr;
}