#include "triangle.h"
#include "imageBuffer.h"
#include "float4x4.h"
#include "shader.h"

#include "../../utils/public/macros.h"

#include <limits>

namespace helper
{
	float EdgeFunction( const float4& a, const float4& b, const float4& c )
	{
		float result = ( c.x - a.x ) * ( b.y - a.y )
			- ( c.y - a.y ) * ( b.x - a.x );

		return result;
	}

	Int32 Clamp( Int32 min, Int32 max, Int32 val )
	{
		SC_ASSERT( min <= max, "" );
		if ( val < min )
			return min;

		if ( val > max )
			return max;

		return val;
	}

	Float Min3f( Float a, Float b, Float c )
	{
		return std::fminf( a, std::fminf( b, c ) );
	}

	Float Max3f( Float a, Float b, Float c )
	{
		return std::fmaxf( a, std::fmaxf( b, c ) );
	}

	inline float4 ConvertToScreenSpace( const ImageBuffer* buffer,
										const float4x4& mvp,
										const float4 wsPos )
	{
		float4 result;
		result = wsPos;
		result.w = 1;

		result = float4x4::Mul( mvp, result );
		result *= 1.f / result.w;

		result.x = ( result.x + 1.f ) * .5f * buffer->Width();
		result.y = ( result.y + 1.f ) * .5f * buffer->Height();
		result.z = 1.f / result.z;
		result.w = 1.0f;

		return result;
	}

	inline void ConvertToScreenSpace( const ImageBuffer* buffer,
									  const float4x4& mvp,
									  const float4x4& model,
									  const float4 msPos[ 3 ],
									  float4 ssPos[ 3 ],
									  float4 wsPos[ 3 ] )
	{
		ssPos[ 0 ] = ConvertToScreenSpace( buffer, mvp, msPos[ 0 ] );
		ssPos[ 1 ] = ConvertToScreenSpace( buffer, mvp, msPos[ 1 ] );
		ssPos[ 2 ] = ConvertToScreenSpace( buffer, mvp, msPos[ 2 ] );

		wsPos[ 0 ] = float4x4::Mul( model, msPos[ 0 ] );
		wsPos[ 1 ] = float4x4::Mul( model, msPos[ 1 ] );
		wsPos[ 2 ] = float4x4::Mul( model, msPos[ 2 ] );
	}
}

CTriangle::CTriangle()
{
}

CTriangle::CTriangle( const float4 & a, const float4 & b, const float4 & c )
{
	m_verts[ 0 ] = a;
	m_verts[ 1 ] = b;
	m_verts[ 2 ] = c;
}

void CTriangle::SetVertexPos( Uint32 index, const float4& pos )
{
	SC_ASSERT( index < 3, "Triangle has only 3 verts!" );
	m_verts[ index ] = pos;
}

void CTriangle::SetVertexColor( Uint32 index, Color col )
{
	SC_ASSERT( index < 3, "Triangle has only 3 verts!" );
	m_vertexColors[ index ] = col;
}

void CTriangle::ComputeNormal()
{
	const float4 u = m_verts[ 1 ] - m_verts[ 0 ];
	const float4 v = m_verts[ 2 ] - m_verts[ 1 ];

	m_normal.x = u.y * v.z - u.z * v.y;
	m_normal.y = u.z * v.x - u.x * v.z;
	m_normal.z = u.x * v.y - u.y * v.x;
	m_normal.Normalize();
	m_normal.w = 0.f;
}

void TestMath()
{
	const float4 rows0[ 4 ] = {
		{ 1, 2, 3, 4 },
		{ 5, 6, 7, 8 },
		{ 9, 10, 11, 12 },
		{ 13, 14, 15, 16 } };

	float4x4 mat0( rows0 );
	float4x4 mat1( rows0 );
	mat1.Transpose();

	float4 mul( 0, 3, 6 );
	mul = float4x4::Mul( mat0, mul );
}

void CTriangle::Draw( ImageBuffer* buffer, Shader* shader ) const
{
	shader->SetNormal( float4x4::Mul( shader->GetMV(), m_normal ).Normalized() );

	float4 ssVerts[ 3 ];
	float4 worldVerts[ 3 ];
	helper::ConvertToScreenSpace( buffer, shader->GetMVP(), shader->GetMV(), m_verts, ssVerts, worldVerts );

	// calculate bounds
	const Float xMin = helper::Min3f( ssVerts[ 0 ].x, ssVerts[ 1 ].x, ssVerts[ 2 ].x );
	const Float yMin = helper::Min3f( ssVerts[ 0 ].y, ssVerts[ 1 ].y, ssVerts[ 2 ].y );

	const Float xMax = helper::Max3f( ssVerts[ 0 ].x, ssVerts[ 1 ].x, ssVerts[ 2 ].x );
	const Float yMax = helper::Max3f( ssVerts[ 0 ].y, ssVerts[ 1 ].y, ssVerts[ 2 ].y );

	// clamp bounds
	const Uint32 minX = helper::Clamp( 0, buffer->Width(), ( Int32 )std::floor( xMin ) );
	const Uint32 minY = helper::Clamp( 0, buffer->Height(), ( Int32 )std::floor( yMin ) );
	const Uint32 maxX = helper::Clamp( 0, buffer->Width(), ( Int32 )std::floor( xMax ) );
	const Uint32 maxY = helper::Clamp( 0, buffer->Height(), ( Int32 )std::floor( yMax ) );

	// rasterization loop
	const Float invArea = 1.f / helper::EdgeFunction( ssVerts[ 0 ], ssVerts[ 1 ], ssVerts[ 2 ] );
	float4 pixelPos;

	for ( Uint32 y = minY; y < maxY; ++y )
	{
		for ( Uint32 x = minX; x < maxX; ++x )
		{
			float4 pixelPos;
			pixelPos.x = static_cast< Float > ( x );
			pixelPos.y = static_cast< Float > ( y );

			// check intersections
			Float w0 = helper::EdgeFunction( ssVerts[ 1 ], ssVerts[ 2 ], pixelPos );
			Float w1 = helper::EdgeFunction( ssVerts[ 2 ], ssVerts[ 0 ], pixelPos );
			Float w2 = helper::EdgeFunction( ssVerts[ 0 ], ssVerts[ 1 ], pixelPos );

			if ( w0 >= 0 && w1 >= 0 && w2 >= 0 )
			{
				w0 *= invArea;
				w1 *= invArea;
				w2 *= invArea;

				// calculate barycentric coordinates
				Color color =
					w0 * m_vertexColors[ 0 ]
					+ w1 * m_vertexColors[ 1 ]
					+ w2 * m_vertexColors[ 2 ];

				const Float oneOverZ =
					ssVerts[ 0 ].z * w0 +
					ssVerts[ 1 ].z * w1 +
					ssVerts[ 2 ].z * w2;

				const Float depth = 1.f / oneOverZ;

				if ( depth > 0.1f && depth < 1000.f )
				{
					if ( depth < buffer->GetDepth( x, y ) )
					{
						pixelPos = worldVerts[ 0 ] * w0
							+ worldVerts[ 1 ] * w1
							+ worldVerts[ 2 ] * w2;

						color = shader->PixelShader( color, pixelPos );
						buffer->WriteColor( x, y, color.GetARGB8() );
						buffer->WriteDepth( x, y, depth );
					}
				}
			}
		}
	}
}