#pragma once
#include "float4.h"
#include "color.h"

#include "../../utils/public/types.h"

class ImageBuffer;
class Shader;

class CTriangle
{
public:
	CTriangle();
	CTriangle( const float4& a, const float4& b, const float4& c );

	void SetVertexPos( Uint32 index, const float4& pos );
	void Draw( ImageBuffer* buffer, Shader* shader ) const;
	void SetVertexColor( Uint32 index, Color col );
	void ComputeNormal();

private:
	float4	m_verts[ 3 ];
	Color	m_vertexColors[ 3 ];
	float4	m_normal;
};