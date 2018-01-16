#pragma once
#include <cmath>

#define CLAMPF_01( x ) ( x > 1.0f ) ? 1.0f : ( x < 0.0f ) ? 0.0f : x;

struct float4
{
	float x, y, z, w;

	float4()
		: x(0.f), y(0.f), z(0.f), w(1.f)
	{ }

	float4(const float4& other)
	{
		Set(other.x, other.y, other.z, other.w);
	}

	float4(float x, float y, float z = 0.f, float w = 1.f)
		: x(x), y(y), z(z), w(w)
	{ }

	inline void operator=(const float4& b)
	{
		Set(b.x, b.y, b.z, b.w);
	}

	inline void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline float Magnitude() const
	{
		return std::sqrtf( SqrMagnitude() );
	}

	inline float Magnitude4() const
	{
		return std::sqrtf( SqrMagnitude4() );
	}

	inline float SqrMagnitude4() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	inline float* GetPtr() 
	{
		return reinterpret_cast< float* >( this );
	}

	inline const float* GetPtr() const
	{
		return &x;
	}

	inline void Saturate()
	{
		x = CLAMPF_01( x );
		y = CLAMPF_01( y );
		z = CLAMPF_01( z );
		w = CLAMPF_01( w );
	}

	inline float& operator[] (int n)
	{
		return (&x)[n];
	}

	inline float operator[] (int n) const 
	{
		return (&x)[n];
	}

	inline void operator+=(const float4& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
	}

	inline float4 operator+(const float4& b) const
	{
		return float4(x + b.x, y + b.y, z + b.z, w + b.w);
	}

	inline void operator-=(const float4& b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
	}

	inline float4 operator-(const float4& b) const
	{
		return float4(x - b.x, y - b.y, z - b.z, w - b.w);
	}

	inline bool operator==(const float4& b) const
	{
		if (x == b.x &&
			y == b.y &&
			z == b.z &&
			w == b.w)
			return true;

		return false;
	}

	inline bool operator!=(const float4& b) const
	{
		return !(*this == b);
	}

	inline void operator*=(float val)
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;
	}

	inline float4 operator*(float val) const
	{
		float4 result = *this;
		result *= val;
		return result;
	}

	inline void operator*=(const float4& b)
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
	}

	inline float4 operator*(const float4& b)
	{
		return float4(x * b.x,
			y * b.y,
			z * b.z,
			w * b.w);
	}

	inline void operator/=(float val)
	{
		const float invVal = 1.0f / val;
		x *= invVal;
		w *= invVal;
		z *= invVal;
		w *= invVal;
	}

	inline float4 operator/(const float val) const
	{
		const float invVal = 1.0f / val;
		float4 result(x * invVal,
			y * invVal,
			z * invVal,
			w * invVal);

		return result;
	}

	inline float4 operator-() const
	{
		float4 result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		result.w = -w;
		return result;
	}

	inline float4 Reflected(const float4& axisNormal) const
	{
		float4 result = (*this) - axisNormal * (2.0f * axisNormal.Dot(*this));
		return result;
	}

	inline float Dot(const float4& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}

	inline float Dot4( const float4& b ) const
	{
		return x * b.x + y * b.y + z * b.z + w * b.w;
	}

	static inline float4 Cross(const float4& a, const float4& b)
	{
		float4 result(
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x));

		return result;
	}

	inline float4 Normalized() const
	{
		const float mag = Magnitude();
		if (mag == 0.f)
			return float4(); // ZERO-vector

		const float invMag = 1.0f / mag;
		float4 result = (*this) * invMag;
		return result;
	}

	inline void Normalize()
	{
		const float mag = Magnitude();
		if (mag == 0.0f)
			return;

		const float invMag = 1.0f / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}

	inline void Lerp(const float4& to, float t)
	{
		(*this) = (*this) + (to - (*this)) * t;
	}
};

inline float4 operator*(float val, const float4& b)
{
	return float4(b * val);
}