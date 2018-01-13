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
	void UpdateMVP();

	void SetNormal( const float4& normal ) { m_normal = normal; }
	Color PixelShader( Color col, const float4& pos ) const;

private:
	// vertex shader
	float4x4 m_viewProjection;
	float4x4 m_model;
	float4x4 m_mvp;
	
	// pixel shader
	float4 m_normal;

	bool m_mvpDirty;
};