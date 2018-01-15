#include "triangle.h"
#include "imageBuffer.h"
#include "float4x4.h"
#include "shader.h"

#include "../../utils/public/macros.h"

#include <limits>

namespace helper
{
	float4 ImageToScreenPos( int x, int y, ImageBuffer* buffer )
	{
		float4 result;
		result.x = ( float )x * buffer->GetAspectRatio() / buffer->Width();
		result.y = ( float )y / buffer->Height();
		return result;
	}

	float EdgeFunction( const float4& a, const float4& b, const float4& p )
	{
		float result = ( p.x - a.x ) * ( b.y - a.y )
			- ( p.y - a.y ) * ( b.x - a.x );

		return result;
	}

	Bool Intersects( const float4* const tri, const float4& point )
	{
		if ( helper::EdgeFunction( tri[ 0 ], tri[ 1 ], point ) > 0.0f )
			return false;

		if ( helper::EdgeFunction( tri[ 1 ], tri[ 2 ], point ) > 0.0f )
			return false;

		if ( helper::EdgeFunction( tri[ 2 ], tri[ 0 ], point ) > 0.0f )
			return false;

		return true;
	}

	Float GetMinOfCoord( Uint32 coordIndex, const float4* const vectors, Uint32 n )
	{
		float minValue = std::numeric_limits< float >::max();
		for ( Uint32 i = 0; i < n; ++i )
		{
			float curr = vectors[ i ][ coordIndex ];
			if ( curr < minValue )
			{
				curr = minValue;
			}
		}

		return minValue;
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

	inline void ConvertToScreenSpace( const ImageBuffer* buffer, 
									  const float4x4& mvp, 
									  const float4 wsPos[ 3 ], 
									  float4 ssPos[ 3 ] )
	{
		for ( int i = 0; i < 3; ++i )
		{
			//shader->VertexShader( m_verts[ i ], ssVerts[ i ] );
			ssPos[ i ] = float4x4::Mul( mvp, wsPos[ i ] );
			ssPos[ i ].x *= buffer->Width();
			ssPos[ i ].y *= buffer->Height();
			ssPos[ i ].z = 1.f / ssPos[ i ].z;
			//ssVerts[ i ].w = 1.f;
		}
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
	//TestMath();
	float4x4 proj;
	proj.SetProjection( 65.f, buffer->GetAspectRatio(), 0.1f, 1000.f );

	float4x4 view;
	view.LookAt( float4( 0, 0, 10.3f ), float4( 0, 0, -1, 0 ), float4( 0, 1, 0, 0 ) );

	shader->SetViewProjectionMatrix( view, proj );

	float4x4 model = float4x4::Identity();
	model.SetTranslation( { 0.2f, 0.2f, 10.f }  );
	float4x4 mvp = proj * view * model;

	float4 ssVerts[ 3 ];
	helper::ConvertToScreenSpace( buffer, mvp, m_verts, ssVerts );

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

	// calculate normal
	//const float4 normal = float4::Cross( m_verts[ 0 ], m_verts[ 1 ] ).Normalized();
	//shader->SetNormal( normal );

	// rasterization loop
	Float invArea = 1.f / helper::EdgeFunction( ssVerts[ 0 ], ssVerts[ 1 ], ssVerts[ 2 ] );

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
						const float4 pixelPos =
							m_verts[ 0 ] * w0
							+ m_verts[ 1 ] * w1
							+ m_verts[ 2 ] * w2;

						//color = shader->PixelShader( color, pixelPos );
						buffer->WriteColor( x, y, color.GetARGB8() );
						buffer->WriteDepth( x, y, depth );
					}
				}
			}
		}
	}
}