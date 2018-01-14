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
}

CTriangle::CTriangle()
{ }

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
	TestMath();
	float4 ssVerts[ 3 ];

	float4x4 proj;
	proj.SetProjection( 90.f, buffer->GetAspectRatio(), 0.1f, 1000.f );

	float4x4 view;
	view.LookAt( float4( 0, 0, 1 ), float4( 0, 0, -1 ), float4( 0, 1, 0 ) );

	shader->SetViewProjectionMatrix( view, proj );

	float4x4 model = float4x4::Identity();
	static float rot = 0;
	rot += 1.5f;

	model.SetRotation( rot, float4( 0, 1, 0 ) );
	shader->SetModelMatrix( model );

	// convert vertices to screen space
	for ( int i = 0; i < 3; ++i )
	{
		shader->VertexShader( m_verts[ i ], ssVerts[ i ] );
		ssVerts[ i ].x *= buffer->Width();
		ssVerts[ i ].y *= buffer->Height();
		ssVerts[ i ].w = 1.f;
	}

	// calculate bounds
	Int32 minX = ( Int32 )ssVerts[ 0 ].x;
	Int32 minY = ( Int32 )ssVerts[ 0 ].y;

	Int32 maxX = ( Int32 )ssVerts[ 0 ].x;
	Int32 maxY = ( Int32 )ssVerts[ 0 ].y;

	for ( Uint32 i = 1; i < 3; ++i )
	{
		// min
		if ( ssVerts[ i ].x < minX )
			minX = ( Int32 )ssVerts[ i ].x;

		if ( ssVerts[ i ].y < minY )
			minY = ( Int32 )ssVerts[ i ].y;

		// max
		if ( ssVerts[ i ].x > maxX )
			maxX = ( Int32 )ssVerts[ i ].x;

		if ( ssVerts[ i ].y > maxY )
			maxY = ( Int32 )ssVerts[ i ].y;
	}

	// clamp bounds
	minX = helper::Clamp( 0, buffer->Width(), minX );
	minY = helper::Clamp( 0, buffer->Height(), minY );
	maxX = helper::Clamp( 0, buffer->Width(), maxX );
	maxY = helper::Clamp( 0, buffer->Height(), maxY );

	// calculate barycentric constants
	const Float dx01 = ssVerts[ 0 ].x - ssVerts[ 1 ].x;
	const Float dx12 = ssVerts[ 1 ].x - ssVerts[ 2 ].x;
	const Float dx20 = ssVerts[ 2 ].x - ssVerts[ 0 ].x;
	const Float dx21 = ssVerts[ 2 ].x - ssVerts[ 1 ].x;

	const Float dy01 = ssVerts[ 0 ].y - ssVerts[ 1 ].y;
	const Float dy12 = ssVerts[ 1 ].y - ssVerts[ 2 ].y;
	const Float dy20 = ssVerts[ 2 ].y - ssVerts[ 0 ].y;

	const Float detTInv = 1.0f / ( dx12 * dy20 - dx20 * dy12 );

	Float bcc[ 3 ]; // barycentric coordinates

	// calculate normal
	const float4 normal = float4::Cross( m_verts[ 0 ], m_verts[ 1 ] ).Normalized();
	shader->SetNormal( normal );

	// rasterization loop
	float depth = 0.0f;
	float4 pixelPos;

	for ( Int32 y = minY; y < maxY; ++y )
	{
		for ( Int32 x = minX; x < maxX; ++x )
		{
			pixelPos.x = static_cast< Float > ( x );
			pixelPos.y = static_cast< Float > ( y );

			// calculate barycentric coordinates
			{
				bcc[ 0 ] =
					detTInv *
					(
						dy12 * ( pixelPos.x - ssVerts[ 2 ].x )
						+ dx21 * ( pixelPos.y - ssVerts[ 2 ].y )
						);

				bcc[ 1 ] =
					detTInv *
					(
						dy20 * ( pixelPos.x - ssVerts[ 2 ].x )
						+ dx20 * ( pixelPos.y - ssVerts[ 2 ].y )
						);

				bcc[ 2 ] = 1.0f - bcc[ 0 ] - bcc[ 1 ];
			}

			Color color =
				bcc[ 0 ] * m_vertexColors[ 0 ]
				+ bcc[ 1 ] * m_vertexColors[ 1 ]
				+ bcc[ 2 ] * m_vertexColors[ 2 ];


			Uint32 argb = color.GetARGB8();

			// check intersections
			if ( helper::Intersects( ssVerts, pixelPos ) )
			{
				depth =
					bcc[ 0 ] * ssVerts[ 0 ].z
					+ bcc[ 1 ] * ssVerts[ 1 ].z
					+ bcc[ 2 ] * ssVerts[ 2 ].z;

				if ( depth < buffer->GetDepth( x, y ) )
				{
					float4 pixelPos = 
						m_verts[ 0 ] * bcc[ 0 ]
						+ m_verts[ 1 ] * bcc[ 1 ]
						+ m_verts[ 2 ] * bcc[ 2 ];

					color = shader->PixelShader( color, pixelPos );
					buffer->WriteColor( x, y, color.GetARGB8() );
					buffer->WriteDepth( x, y, depth );
				}
			}
		}
	}
}