#pragma once

#include "types.h"
#include <cmath>

struct Vector4
{
	Float x, y, z, w;

	Vector4()
		: x( 0.f ), y( 0.f ), z( 0.f ), w( 0.f )
	{ }

	Vector4( const Vector4& other )
	{
		Set( other.x, other.y, other.z, other.w );
	}

	Vector4( Float x, Float y, Float z = 0.f, Float w = 0.f )
		: x( x ), y( y ), z( z ), w( w )
	{ }

	inline void operator=( const Vector4& b )
	{
		Set( b.x, b.y, b.z, b.w );
	}

	inline void Set( Float x, Float y, Float z, Float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline Float Magnitude() const
	{
		return std::sqrtf( SqrMagnitude() );
	}

	inline Float SqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float* GetPtr()
	{
		return &x;
	}

	inline const float*  GetPtr() const
	{
		return &x;
	}

	inline float& operator[] ( Uint32 n )
	{
		return ( &x )[ n ];
	}

	inline Float operator[] ( Uint32 n ) const
	{
		return ( &x )[ n ];
	}

	inline void operator+=( const Vector4& b )
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
	}

	inline Vector4 operator+( const Vector4& b ) const
	{
		return Vector4( x + b.x, y + b.y, z + b.z, w + b.w );
	}

	inline void operator-=( const Vector4& b )
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
	}

	inline Vector4 operator-( const Vector4& b ) const
	{
		return Vector4( x - b.x, y - b.y, z - b.z, w - b.w );
	}

	inline bool operator==( const Vector4& b ) const
	{
		if ( x == b.x &&
			 y == b.y &&
			 z == b.z &&
			 w == b.w )
			return true;

		return false;
	}

	inline bool operator!=( const Vector4& b ) const
	{
		return !( *this == b );
	}

	inline void operator*=( Float val )
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;
	}

	inline Vector4 operator*( Float val ) const
	{
		Vector4 result = *this;
		result *= val;
		return result;
	}

	inline void operator*=( const Vector4& b )
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
	}

	inline Vector4 operator*( const Vector4& b )
	{
		return Vector4( x * b.x,
					   y * b.y,
					   z * b.z,
					   w * b.w );
	}

	inline void operator/=( Float val )
	{
		const Float invVal = 1.0f / val;
		x *= invVal;
		w *= invVal;
		z *= invVal;
		w *= invVal;
	}

	inline Vector4 operator/( const Float val ) const
	{
		const Float invVal = 1.0f / val;
		Vector4 result( x * invVal,
					   y * invVal,
					   z * invVal,
					   w * invVal );

		return result;
	}

	inline Vector4 operator-() const
	{
		Vector4 result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		result.w = -w;
		return result;
	}

	inline Vector4 Reflected( const Vector4& axisNormal ) const
	{
		Vector4 result = ( *this ) - axisNormal * ( 2.0f * axisNormal.Dot( *this ) );
		return result;
	}

	inline Float Dot( const Vector4& b ) const
	{
		return x * b.x + y * b.y + z * b.z + w * b.w;
	}

	static inline Vector4 Cross( const Vector4& a, const Vector4& b )
	{
		Vector4 result(
			( a.y * b.z ) - ( a.z * b.y ),
			( a.z * b.x ) - ( a.x * b.z ),
			( a.x * b.y ) - ( a.y * b.x ) );

		return result;
	}

	inline Vector4 Normalized() const
	{
		const Float mag = Magnitude();
		if ( mag == 0.f )
			return Vector4(); // ZERO-vector

		const Float invMag = 1.0f / mag;
		Vector4 result = ( *this ) * invMag;
		return result;
	}

	inline void Normalize()
	{
		const Float mag = Magnitude();
		if ( mag == 0.0f )
			return;

		const Float invMag = 1.0f / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}

	inline void Lerp( const Vector4& to, Float t )
	{
		( *this ) = ( *this ) + ( to - ( *this ) ) * t;
	}
};

inline Vector4 operator*( Float val, const Vector4& b )
{
	return Vector4( b * val );
}