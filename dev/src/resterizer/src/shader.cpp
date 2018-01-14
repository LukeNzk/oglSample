#include "shader.h"

Shader::Shader()
	: m_mvpDirty( true )
{
}

void Shader::SetViewProjectionMatrix( const float4x4& view, const float4x4& projection )
{
	m_view = view;
	m_projection = projection;
	m_mvpDirty = true;
}

void Shader::SetModelMatrix( const float4x4& model )
{
	m_model = model;
	m_mvpDirty = true;
}

void Shader::UpdateMVP()
{
	m_mvp = m_projection * m_view * m_model;;
	m_mvpDirty = false;
}

Color Shader::PixelShader( Color col, const float4& pos ) const
{
	const float4 lightPos( 1, 1, 1 );

	return col;
	const float4 lToP = pos - lightPos;
	const float magSqr = lToP.SqrMagnitude();

	float light = 2.f - magSqr;
	if ( light < 0.0f )
		light = 0.0f;
	if ( light > 1.0f )
		light = 1.0f;

	col.Set( ( Uint8 )( col.r * light ), ( Uint8 )( col.g * light ), ( Uint8 )( col.b * light ), col.a );

	return col;
}

void Shader::VertexShader( const float4& vert, float4& dst )
{
	if ( m_mvpDirty )
		UpdateMVP();

	dst = float4x4::Mul( m_mvp, vert );
}
