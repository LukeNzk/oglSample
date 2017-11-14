#pragma once

#include "types.h"
#include <cmath>

struct Vector2
{
	Float x, y;

	Vector2()
		: x( 0.f ), y( 0.f )
	{ }

	Vector2( const Vector2& other )
	{
		Set( other.x, other.y );
	}

	Vector2( Float x, Float y )
		: x( x ), y( y )
	{ }

	inline void operator=( const Vector2& b )
	{
		Set( b.x, b.y );
	}

	inline void Set( Float x, Float y )
	{
		this->x = x;
		this->y = y;
	}

	inline Float Magnitude() const
	{
		return std::sqrtf( SqrMagnitude() );
	}

	inline Float SqrMagnitude() const
	{
		return x * x + y * y;
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

	inline void operator+=( const Vector2& b )
	{
		x += b.x;
		y += b.y;
	}

	inline Vector2 operator+( const Vector2& b ) const
	{
		return Vector2( x + b.x, y + b.y );
	}

	inline void operator-=( const Vector2& b )
	{
		x -= b.x;
		y -= b.y;
	}

	inline Vector2 operator-( const Vector2& b ) const
	{
		return Vector2( x - b.x, y - b.y );
	}

	inline bool operator==( const Vector2& b ) const
	{
		if ( x == b.x &&
			 y == b.y )
			return true;

		return false;
	}

	inline bool operator!=( const Vector2& b ) const
	{
		return !( *this == b );
	}

	inline void operator*=( Float val )
	{
		x *= val;
		y *= val;
	}

	inline Vector2 operator*( Float val ) const
	{
		Vector2 result = *this;
		result *= val;
		return result;
	}

	inline void operator*=( const Vector2& b )
	{
		x *= b.x;
		y *= b.y;
	}

	inline Vector2 operator*( const Vector2& b )
	{
		return Vector2( x * b.x,
						y * b.y );
	}

	inline void operator/=( Float val )
	{
		const Float invVal = 1.0f / val;
		x *= invVal;
		y *= invVal;
	}

	inline Vector2 operator/( const Float val ) const
	{
		const Float invVal = 1.0f / val;
		Vector2 result( x * invVal,
						y * invVal );

		return result;
	}

	inline Vector2 operator-() const
	{
		Vector2 result;
		result.x = -x;
		result.y = -y;
		return result;
	}

	inline Vector2 Reflected( const Vector2& axisNormal ) const
	{
		Vector2 result = ( *this ) - axisNormal * ( 2.0f * axisNormal.Dot( *this ) );
		return result;
	}

	inline Float Dot( const Vector2& b ) const
	{
		return x * b.x + y * b.y;
	}

	inline Vector2 Normalized() const
	{
		const Float mag = Magnitude();
		if ( mag == 0.f )
			return Vector2(); // ZERO-vector

		const Float invMag = 1.0f / mag;
		Vector2 result = ( *this ) * invMag;
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
	}

	inline void Lerp( const Vector2& to, Float t )
	{
		( *this ) = ( *this ) + ( to - ( *this ) ) * t;
	}
};

inline Vector2 operator*( Float val, const Vector2& b )
{
	return Vector2( b * val );
}