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

	void SetVertexPos( Uint32 index, const float4& pos );
	void Draw( ImageBuffer* buffer, Shader* shader ) const;
	void SetVertexColor( Uint32 index, Color col );

private:
	float4 m_verts[ 3 ];
	Color m_vertexColors[ 3 ];
};