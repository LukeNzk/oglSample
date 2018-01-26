#pragma once

#include "float4x4.h"
#include "color.h"

#include "../../utils/public/types.h"

class CTriangle;

class Shader
{
public:
	Shader();

	void SetViewProjectionMatrix( const float4x4& view, const float4x4& projection );
	void SetModelMatrix( const float4x4& model );

	void VertexShader( const float4& vert, float4& dst );

	const float4x4& GetMVP()
	{
		if (m_mvpDirty)
			UpdateMVP();
		return m_mvp;
	}

	const float4x4& GetMV()
	{
		if ( m_mvpDirty )
			UpdateMVP();
		return m_mv;
	}

	const float4x4& GetModelMatrix() const { return m_model; }

	void UpdateMVP();

	void SetNormal( const float4& normal ) { m_normal = normal; }
	Color PixelShader( Color col, const float4& wsPos ) const;

private:
	struct
	{
		float4 m_pos;
		float4 m_vsPos;
	} m_light;

	// vertex shader
	float4x4 m_view;
	float4x4 m_projection;
	float4x4 m_model;

	float4x4 m_mv;
	float4x4 m_mvp;
	
	// pixel shader
	float4 m_normal;

	bool m_mvpDirty;
};