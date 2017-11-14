#pragma once

#pragma once
#include "xmmintrin.h"
#include "vector4.h"

class Matrix
{
public:
	Matrix()
	{ }

	Matrix( Vector4 rows[ 4 ] )
	{
		r[ 0 ] = rows[ 0 ];
		r[ 1 ] = rows[ 1 ];
		r[ 2 ] = rows[ 2 ];
		r[ 3 ] = rows[ 3 ];
	}

	Matrix( Float m0, Float m1, Float m2, Float m3,
			Float m4, Float m5, Float m6, Float m7,
			Float m8, Float m9, Float m10, Float m11,
			Float m12, Float m13, Float m14, Float m15 )
	{
		Float* ptr = &r[ 0 ].x;
		ptr[ 0 ] = m0;
		ptr[ 1 ] = m1;
		ptr[ 2 ] = m2;
		ptr[ 3 ] = m3;
		ptr[ 4 ] = m4;
		ptr[ 5 ] = m5;
		ptr[ 6 ] = m6;
		ptr[ 7 ] = m7;
		ptr[ 8 ] = m8;
		ptr[ 9 ] = m9;
		ptr[ 10 ] = m10;
		ptr[ 11 ] = m11;
		ptr[ 12 ] = m12;
		ptr[ 14 ] = m14;
		ptr[ 15 ] = m15;
	}

	void Dot( const Matrix& other, Matrix& dst )
	{
		Matrix transposed;
		other.Transpose( transposed );

		for ( int i = 0; i < 4; ++i )
		{
			dst[ i ][ 0 ] = r[ i ].Dot( transposed[ 0 ] );
			dst[ i ][ 1 ] = r[ i ].Dot( transposed[ 1 ] );
			dst[ i ][ 2 ] = r[ i ].Dot( transposed[ 2 ] );
			dst[ i ][ 3 ] = r[ i ].Dot( transposed[ 3 ] );
		}
	}

	void GetColumn( Uint32 n, Vector4& dst )
	{
		dst.x = r[ 0 ][ n ];
		dst.y = r[ 1 ][ n ];
		dst.z = r[ 2 ][ n ];
		dst.w = r[ 3 ][ n ];
	}

	void Transpose()
	{
		__m128 row0 = _mm_load_ps( r[ 0 ].GetPtr() );
		__m128 row1 = _mm_load_ps( r[ 1 ].GetPtr() );
		__m128 row2 = _mm_load_ps( r[ 2 ].GetPtr() );
		__m128 row3 = _mm_load_ps( r[ 3 ].GetPtr() );
		_MM_TRANSPOSE4_PS( row0, row1, row2, row3 );
		_mm_store_ps( r[ 0 ].GetPtr(), row0 );
		_mm_store_ps( r[ 1 ].GetPtr(), row1 );
		_mm_store_ps( r[ 2 ].GetPtr(), row2 );
		_mm_store_ps( r[ 3 ].GetPtr(), row3 );
	}

	void Transpose( Matrix& dst ) const
	{
		__m128 row0 = _mm_load_ps( r[ 0 ].GetPtr() );
		__m128 row1 = _mm_load_ps( r[ 1 ].GetPtr() );
		__m128 row2 = _mm_load_ps( r[ 2 ].GetPtr() );
		__m128 row3 = _mm_load_ps( r[ 3 ].GetPtr() );
		_MM_TRANSPOSE4_PS( row0, row1, row2, row3 );
		_mm_store_ps( dst[ 0 ].GetPtr(), row0 );
		_mm_store_ps( dst[ 1 ].GetPtr(), row1 );
		_mm_store_ps( dst[ 2 ].GetPtr(), row2 );
		_mm_store_ps( dst[ 3 ].GetPtr(), row3 );
	}

	Vector4& operator[]( int n )
	{
		return r[ n ];
	}

	const Vector4& operator[]( int n ) const
	{
		return r[ n ];
	}

	static Matrix CreateOrthoProj( Float l, Float b, Float r, Float t, Float n, Float f )
	{
		const Float invRL = 1.0f / ( r - l );
		const Float invTB = 1.0f / ( t - b );
		const Float invFN = 1.0f / ( f - n );

		Matrix mat( 2.0f * invRL, 0.0f, 0.0f, 0.0f, 
					0.0f, 2.0f * invTB, 0.0f, 0.0f,
					0.0f, 0.0f, 2.0f * invFN, 0.0f,
					-( r + l ) * invRL, -( t + b ) * invTB, -( f + n ) * invFN, 1.0f );

		return mat;
	}

	const Float* GetData() const { return &r[ 0 ].x; }

private:
	Vector4 r[ 4 ];
};