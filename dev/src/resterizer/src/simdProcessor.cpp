#include "simdProcessor.h"

namespace helper
{
	inline void EdgeFunction( const QVec3* a, const QVec3* b, const QVec3* c, __m128& result )
	{
		//r = ( c.x - a.x ) * ( b.y - a.y ) - ( c.y - a.y ) * ( b.x - a.x );

		__m128 r1 = _mm_sub_ps( c->mm.x, a->mm.x ); //( c.x - a.x )
		__m128 r2 = _mm_sub_ps( b->mm.y, a->mm.y ); //( b.y - a.y )

		result = _mm_mul_ps( r1, r2 );

		r1 = _mm_sub_ps( c->mm.y, a->mm.y ); //( c.y - a.y )
		r2 = _mm_sub_ps( b->mm.x, a->mm.x ); //( b.x - a.x )

		result = _mm_sub_ps( result, _mm_mul_ps( r1, r2 ) );
	}
}

void SIMDProcessor::ComputeEdges()
{
	helper::EdgeFunction( &m_ssVerts[ 1 ], &m_ssVerts[ 2 ], &m_pixelPos, m_bcc.mm.x );
	helper::EdgeFunction( &m_ssVerts[ 2 ], &m_ssVerts[ 0 ], &m_pixelPos, m_bcc.mm.y );
	helper::EdgeFunction( &m_ssVerts[ 0 ], &m_ssVerts[ 1 ], &m_pixelPos, m_bcc.mm.z );
}

void SIMDProcessor::TestEdges()
{
	static const __m128 zero = { 0 };
	m_pixelCol.mm.x = _mm_cmpge_ps( m_bcc.mm.x, zero );
	m_pixelCol.mm.y = _mm_cmpge_ps( m_bcc.mm.y, zero );
	m_pixelCol.mm.z = _mm_cmpge_ps( m_bcc.mm.z, zero );
}

void SIMDProcessor::BccMulInvArea()
{
	m_bcc.mm.x = _mm_mul_ps( m_bcc.mm.x, m_invArea.mm );
	m_bcc.mm.y = _mm_mul_ps( m_bcc.mm.y, m_invArea.mm );
	m_bcc.mm.z = _mm_mul_ps( m_bcc.mm.z, m_invArea.mm );
}

void SIMDProcessor::CalcDepth()
{
	__m128 r1 = _mm_mul_ps( m_bcc.mm.x, m_ssVerts[ 0 ].mm.z );
	__m128 r2 = _mm_mul_ps( m_bcc.mm.y, m_ssVerts[ 1 ].mm.z );
	r1 = _mm_add_ps( r1, r2 );
	r2 = _mm_mul_ps( m_bcc.mm.z, m_ssVerts[ 2 ].mm.z );
	m_oneOverZ.mm = _mm_add_ps( r1, r2 );
}

void SIMDProcessor::CalcPixelPos()
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

void SIMDProcessor::CalcColor()
{
	static const __m128 max = { 255.f, 255.f, 255.f, 255.f };
	static const __m128 min;

	__m128 r1 = _mm_mul_ps( m_bcc.mm.x, m_vColors[ 0 ].mm.r );
	__m128 r2 = _mm_mul_ps( m_bcc.mm.y, m_vColors[ 1 ].mm.r );
	m_color.mm.r = _mm_mul_ps( m_bcc.mm.z, m_vColors[ 2 ].mm.r );

	m_color.mm.r = _mm_add_ps( m_color.mm.r, r1 );
	m_color.mm.r = _mm_add_ps( m_color.mm.r, r2 );

	// clamp [0, 255]
	m_color.mm.r = _mm_min_ps( _mm_max_ps( m_color.mm.r, min ), max );

	r1 = _mm_mul_ps( m_bcc.mm.x, m_vColors[ 0 ].mm.g );
	r2 = _mm_mul_ps( m_bcc.mm.y, m_vColors[ 1 ].mm.g );
	m_color.mm.g = _mm_mul_ps( m_bcc.mm.z, m_vColors[ 2 ].mm.g );

	m_color.mm.g = _mm_add_ps( m_color.mm.g, r1 );
	m_color.mm.g = _mm_add_ps( m_color.mm.g, r2 );

	// clamp [0, 255]
	m_color.mm.g = _mm_min_ps( _mm_max_ps( m_color.mm.g, min ), max );

	r1 = _mm_mul_ps( m_bcc.mm.x, m_vColors[ 0 ].mm.b );
	r2 = _mm_mul_ps( m_bcc.mm.y, m_vColors[ 1 ].mm.b );
	m_color.mm.b = _mm_mul_ps( m_bcc.mm.z, m_vColors[ 2 ].mm.b );

	m_color.mm.b = _mm_add_ps( m_color.mm.b, r1 );
	m_color.mm.b = _mm_add_ps( m_color.mm.b, r2 );

	// clamp [0, 255]
	m_color.mm.b = _mm_min_ps( _mm_max_ps( m_color.mm.b, min ), max );
}
