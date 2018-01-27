#include "triangle.h"
#include "imageBuffer.h"
#include "float4x4.h"
#include "shader.h"

#include "../../utils/public/macros.h"

#include <limits>
#include <xmmintrin.h>

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

class SIMDProcessor
{
public:
	__declspec( align( 16 ) )
	struct QFloat
	{
		union
		{
			__m128 mm;
			Float f[ 4 ];
		};

		void Set( Uint32 index, Float val )
		{
			f[ index ] = val;
		}
	};

	__declspec( align( 16 ) )
	struct QVec3
	{
		union
		{
			struct
			{
				__m128 x;
				__m128 y;
				__m128 z;
			} mm;

			struct
			{
				Float x[ 4 ];
				Float y[ 4 ];
				Float z[ 4 ];
			};
		};

		void Set( Uint32 index, const Float* val )
		{
			x[ index ] = val[ 0 ];
			y[ index ] = val[ 1 ];
			z[ index ] = val[ 2 ];
		}
	};

	QVec3 m_ssVerts[ 3 ];
	QVec3 m_wsVerts[ 3 ];
	QVec3 m_wsPx;

	QVec3 m_pixelPos;
	QVec3 m_bcc; // barycentric
	QVec3 m_pixelCol;

	QFloat m_oneOverZ;
	QFloat m_invArea;

	void EdgeFunction( const QVec3* a, const QVec3* b, const QVec3* c, __m128& result )
	{
		//r = ( c.x - a.x ) * ( b.y - a.y ) - ( c.y - a.y ) * ( b.x - a.x );

		__m128 r1 = _mm_sub_ps( c->mm.x, a->mm.x ); //( c.x - a.x )
		__m128 r2 = _mm_sub_ps( b->mm.y, a->mm.y ); //( b.y - a.y )

		result = _mm_mul_ps( r1, r2 );

		r1 = _mm_sub_ps( c->mm.y, a->mm.y ); //( c.y - a.y )
		r2 = _mm_sub_ps( b->mm.x, a->mm.x ); //( b.x - a.x )

		result = _mm_sub_ps( result, _mm_mul_ps( r1, r2 ) );
	}

	void ComputeEdges()
	{
		EdgeFunction( &m_ssVerts[ 1 ], &m_ssVerts[ 2 ], &m_pixelPos, m_bcc.mm.x );
		EdgeFunction( &m_ssVerts[ 2 ], &m_ssVerts[ 0 ], &m_pixelPos, m_bcc.mm.y );
		EdgeFunction( &m_ssVerts[ 0 ], &m_ssVerts[ 1 ], &m_pixelPos, m_bcc.mm.z );
	}

	void TestEdges()
	{
		static const __m128 zero = { 0 };
		m_pixelCol.mm.x = _mm_cmpge_ps( m_bcc.mm.x, zero );
		m_pixelCol.mm.y = _mm_cmpge_ps( m_bcc.mm.y, zero );
		m_pixelCol.mm.z = _mm_cmpge_ps( m_bcc.mm.z, zero );
	}

	void BccMulInvArea()
	{
		m_bcc.mm.x = _mm_mul_ps( m_bcc.mm.x, m_invArea.mm );
		m_bcc.mm.y = _mm_mul_ps( m_bcc.mm.y, m_invArea.mm );
		m_bcc.mm.z = _mm_mul_ps( m_bcc.mm.z, m_invArea.mm );
	}

	void CalcDepth()
	{
		__m128 r1 = _mm_mul_ps( m_bcc.mm.x, m_ssVerts[ 0 ].mm.z );
		__m128 r2 = _mm_mul_ps( m_bcc.mm.y, m_ssVerts[ 1 ].mm.z );
		r1 = _mm_add_ps( r1, r2 );
		r2 = _mm_mul_ps( m_bcc.mm.z, m_ssVerts[ 2 ].mm.z );
		m_oneOverZ.mm = _mm_add_ps( r1, r2 );
	}

	void CalcPixelPos()
	{
		__m128 r1 = _mm_mul_ps( m_bcc.mm.x, m_wsVerts[ 0 ].mm.x );
		__m128 r2 = _mm_mul_ps( m_bcc.mm.y, m_wsVerts[ 1 ].mm.x );
		m_wsPx.mm.x = _mm_mul_ps( m_bcc.mm.z, m_wsVerts[ 2 ].mm.x );

		m_wsPx.mm.x = _mm_add_ps( m_wsPx.mm.x, r1 );
		m_wsPx.mm.x = _mm_add_ps( m_wsPx.mm.x, r2 );

		r1 = _mm_mul_ps( m_bcc.mm.x, m_wsVerts[ 0 ].mm.y );
		r2 = _mm_mul_ps( m_bcc.mm.y, m_wsVerts[ 1 ].mm.y );
		m_wsPx.mm.y = _mm_mul_ps( m_bcc.mm.z, m_wsVerts[ 2 ].mm.y );

		m_wsPx.mm.y = _mm_add_ps( m_wsPx.mm.y, r1 );
		m_wsPx.mm.y = _mm_add_ps( m_wsPx.mm.y, r2 );

		r1 = _mm_mul_ps( m_bcc.mm.x, m_wsVerts[ 0 ].mm.z );
		r2 = _mm_mul_ps( m_bcc.mm.y, m_wsVerts[ 1 ].mm.z );
		m_wsPx.mm.z = _mm_mul_ps( m_bcc.mm.z, m_wsVerts[ 2 ].mm.z );

		m_wsPx.mm.z = _mm_add_ps( m_wsPx.mm.z, r1 );
		m_wsPx.mm.z = _mm_add_ps( m_wsPx.mm.z, r2 );
	}
};

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
	Color color;
	SIMDProcessor simd;

	for ( int i = 0; i < 4; ++i )
	{
		simd.m_ssVerts[ 0 ].Set( i, &ssVerts[ 0 ].x );
		simd.m_ssVerts[ 1 ].Set( i, &ssVerts[ 1 ].x );
		simd.m_ssVerts[ 2 ].Set( i, &ssVerts[ 2 ].x );
		
		simd.m_wsVerts[ 0 ].Set( i, &worldVerts[ 0 ].x );
		simd.m_wsVerts[ 1 ].Set( i, &worldVerts[ 1 ].x );
		simd.m_wsVerts[ 2 ].Set( i, &worldVerts[ 2 ].x );

		simd.m_invArea.Set( i, invArea );
	}

	Uint32 pixelCount = 0;
	for ( Uint32 y = minY; y < maxY; ++y )
	{
		for ( Uint32 x = minX; x < maxX; ++x )
		{
			pixelPos.x = static_cast< Float > ( x );
			pixelPos.y = static_cast< Float > ( y );

			int simdIndex = ( pixelCount ) % 4;
			++pixelCount;
			if ( simdIndex == 0 )
			{
				float4 mmPixelPos = pixelPos;

				for ( Uint32 i = 0; i < 4; ++i )
				{
					simd.m_pixelPos.Set( i, &mmPixelPos.x );
					if ( x + i == maxX - 1 )
					{
						mmPixelPos.x = static_cast< Float >( minX );
						mmPixelPos.y += 1.f;
					}
					else
					{
						mmPixelPos.x += 1.f;
					}
				}

				simd.ComputeEdges();
				simd.TestEdges();
				simd.BccMulInvArea();
				simd.CalcDepth();
				simd.CalcPixelPos();
			}

			// check intersections
			const Float w0 = simd.m_bcc.x[ simdIndex ];
			const Float w1 = simd.m_bcc.y[ simdIndex ];
			const Float w2 = simd.m_bcc.z[ simdIndex ];

			if ( simd.m_pixelCol.x[ simdIndex ] &&
				 simd.m_pixelCol.y[ simdIndex ] &&
				 simd.m_pixelCol.z[ simdIndex ] )
			{
				color =
					w0 * m_vertexColors[ 0 ]
					+ w1 * m_vertexColors[ 1 ]
					+ w2 * m_vertexColors[ 2 ];

				const Float oneOverZ = simd.m_oneOverZ.f[ simdIndex ];

				if ( oneOverZ > 1.f )
				{
					if ( oneOverZ > buffer->GetDepth( x, y ) )
					{
						pixelPos = { simd.m_wsPx.x[ simdIndex ],
									  simd.m_wsPx.y[ simdIndex ],
									  simd.m_wsPx.z[ simdIndex ], 0.f };

						color = shader->PixelShader( color, pixelPos );
						buffer->WriteColor( x, y, color.GetARGB8() );
						buffer->WriteDepth( x, y, oneOverZ );
					}
				}
			}
		}
	}
}