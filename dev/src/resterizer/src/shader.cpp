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
	m_mvp = m_projection * m_view * m_model;
	m_mv = m_view * m_model;
	m_mvpDirty = false;

	m_light.m_pos = { -2.f, 0.f, 10.f, 1.f };
	m_light.m_vsPos = float4x4::Mul( m_view, m_light.m_pos );
}

Color Shader::PixelShader( Color col, const float4& pos ) const
{
	float4 L = m_light.m_vsPos - pos;
	const Float dist2 = L.SqrMagnitude();
	L.Normalize();

	const Float NdotL = 80 * CLAMPF_01( m_normal.Dot( L ) ) / dist2;
	return ( col * NdotL );
}

void Shader::VertexShader( const float4& vert, float4& dst )
{
	if ( m_mvpDirty )
		UpdateMVP();

	dst = float4x4::Mul( m_mvp, vert );
}
