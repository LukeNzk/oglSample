#pragma once
#include "xmmintrin.h"
#include "float4.h"
#include <stdlib.h>
#include <stdio.h>

#include "../../utils/public/types.h"

#define _USE_MATH_DEFINES
#include <math.h>

class float4x4
{
public:
	float4x4()
	{ }

	float4x4( const float4 rows[ 4 ] )
	{
		r[ 0 ] = rows[ 0 ];
		r[ 1 ] = rows[ 1 ];
		r[ 2 ] = rows[ 2 ];
		r[ 3 ] = rows[ 3 ];
	}

	static float4x4 Mul( const float4x4& a, const float4x4& b )
	{
		float4x4 res = Zero();
		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
			{
				for ( int k = 0; k < 4; k++ )
				{
					res[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];
				}
			}
		}

		return res;
	}

	float4x4 operator*( const float4x4& other )
	{
		return float4x4::Mul( *this, other );
	}

	static float4x4 Zero()
	{
		static const float4 mat[ 4 ] =
		{
			{ 0.f, 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f, 0.f }
		};

		return float4x4( mat );
	}

	void GetColumn( Uint32 n, float4& dst )
	{
		dst.x = r[ 0 ][ n ];
		dst.y = r[ 1 ][ n ];
		dst.z = r[ 2 ][ n ];
		dst.w = r[ 3 ][ n ];
	}

	void Transpose()
	{
		float t = 0.0f;
		for ( int i = 0; i < 3; ++i )
		{
			for ( int j = i + 1; j < 4; ++j )
			{
				t = r[ i ][ j ];
				r[ i ][ j ] = r[ j ][ i ];
				r[ j ][ i ] = t;
			}
		}
	}

	void Transpose( float4x4& dst ) const
	{
		float t = 0.0f;
		for ( int i = 0; i < 3; ++i )
		{
			for ( int j = i + 1; j < 4; ++j )
			{
				t = dst[ i ][ j ];
				dst[ i ][ j ] = dst[ j ][ i ];
				dst[ j ][ i ] = t;
			}
		}
	}

	void SetProjection( Float fov, Float ar, Float near, Float far )
	{
		const Float PIf = 3.14159265358979f / 360.0f;
		fov *= PIf;

		const Float near_FarInv = 1.f / ( near - far );
		const Float f = std::cosf( fov ) / std::sinf( fov );

		r[ 0 ] = float4( f / ar, 0.f, 0.f, 0.f );
		r[ 1 ] = float4( 0.f, f, 0.f, 0.f );
		r[ 2 ] = float4( 0.f, 0.f, ( near + far ) * near_FarInv, 2.f * near * far * near_FarInv );
		r[ 3 ] = float4( 0.f, 0.f, -1.f, 0.f );
	}

	void LookAt( float4 eye, float4 target, float4 up )
	{
		float4 f = ( eye - target );    // forward
		f.Normalize();

		up.Normalize();
		float4 rt = float4::Cross( up, f ); // right
		rt.Normalize();

		float4 u = float4::Cross( f , rt ); // up
		u.Normalize();

		r[ 0 ] = float4( rt.x, rt.y, rt.z, eye.x );
		r[ 1 ] = float4( u.x, u.y, u.z, eye.y );
		r[ 2 ] = float4( f.x, f.y, f.z, eye.z );
		r[ 3 ] = float4( 0.f, 0.f, 0.f, 1.f );
	}

	void SetTranslation( float4 vec )
	{
		r[ 0 ] = float4( 1.f, 0.f, 0.f, vec.x );
		r[ 1 ] = float4( 0.f, 1.f, 0.f, vec.y );
		r[ 2 ] = float4( 0.f, 0.f, 1.f, vec.z );
		r[ 3 ] = float4( 0.f, 0.f, 0.f, 1.f );
	}

	void SetScale( float4 s )
	{
		r[ 0 ] = float4( s.x, 0.f, 0.f, 0.f );
		r[ 1 ] = float4( 0.f, s.y, 0.f, 0.f );
		r[ 2 ] = float4( 0.f, 0.f, s.z, 0.f );
		r[ 3 ] = float4( 0.f, 0.f, 0.f, 1.f );
	}

	void SetRotation( Float a, float4 axis )
	{
		const Float pi180 = 3.14159265358979f / 180.f;
		const Float s = std::sinf( a * pi180 );
		const Float c = std::cosf( a * pi180 );
		const Float oneC = 1.f - c;

		r[ 0 ] = float4( axis.x * axis.x * oneC + c,
						 axis.x * axis.y * oneC - axis.z * s,
						 axis.x * axis.z * oneC + axis.y * s,
						 0.f );

		r[ 1 ] = float4( axis.y * axis.x * oneC + axis.z * s,
						 axis.y * axis.y * oneC + c,
						 axis.y * axis.z * oneC - axis.x * s,
						 0.f );

		r[ 2 ] = float4( axis.z * axis.x * oneC - axis.y * s,
						 axis.z * axis.y * oneC + axis.x * s,
						 axis.z * axis.z * oneC + c,
						 0.f );

		r[ 3 ] = float4( 0.f, 0.f, 0.f, 1.f );
	}

	static float4x4 Identity()
	{
		const float4 rows[ 4 ] = { 
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 } };

		return float4x4( rows );
	}

	static float4 Mul( const float4x4& mat, const float4& vec )
	{
		float4 result;

		result[ 0 ] =
			vec.x * mat[ 0 ][ 0 ]
			+ vec.y * mat[ 0 ][ 1 ]
			+ vec.z * mat[ 0 ][ 2 ]
			+ vec.w * mat[ 0 ][ 3 ];

		result[ 1 ] =
			vec.x * mat[ 1 ][ 0 ]
			+ vec.y * mat[ 1 ][ 1 ]
			+ vec.z * mat[ 1 ][ 2 ]
			+ vec.w * mat[ 1 ][ 3 ];

		result[ 2 ] =
			vec.x * mat[ 2 ][ 0 ]
			+ vec.y * mat[ 2 ][ 1 ]
			+ vec.z * mat[ 2 ][ 2 ]
			+ vec.w * mat[ 2 ][ 3 ];

		result[ 3 ] =
			vec.x * mat[ 3 ][ 0 ]
			+ vec.y * mat[ 3 ][ 1 ]
			+ vec.z * mat[ 3 ][ 2 ]
			+ vec.w * mat[ 3 ][ 3 ];

		return result;
	}

	float4& operator[]( int n )
	{
		return r[ n ];
	}

	const float4& operator[]( int n ) const
	{
		return r[ n ];
	}

private:
	float4 r[ 4 ];
};