#pragma once
#include "float4x4.h"

#include <vector>

class CTriangle;
class ImageBuffer;
class Shader;

class Model
{
public:
	Model();
	~Model();
	
	float4x4 GetTransform( float angle, const float4& axis )
	{
		return m_transform;
	}

	static Model* CreateCube( const float4& pos, const float4& size );

	void Draw( ImageBuffer* buffer, Shader* shader ) const;

	void AddTriangle( const float4& a, const float4& b, const float4& c );

private:
	float4x4					m_transform;
	std::vector< CTriangle* >	m_triangles;
};