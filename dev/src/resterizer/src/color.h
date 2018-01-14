#pragma once
#include "../../utils/public/types.h"

class Color
{
public:
	Color()
	{
		Set( 0, 0, 0 );
	}

	Color( Uint32 col )
	{
		m_rgba8 = col;
	}

	Color( Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xff )
	{
		Set( r, g, b, a );
	}

	inline void Set( Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xff )
	{
		m_bgra[ 0 ] = b;
		m_bgra[ 1 ] = g;
		m_bgra[ 2 ] = r;
		m_bgra[ 3 ] = a;
	}

	inline Uint32 GetARGB8() const 
	{ 
		return m_rgba8;
	}

	static Color Red() { return Color( 255, 0, 0 ); }
	static Color Green() { return Color( 0, 255, 0 ); }
	static Color Blue() { return Color( 0, 0, 255 ); }

public:
	union
	{
		Uint8 m_bgra[ 4 ]; // little-endian
		Uint32 m_rgba8;
		struct
		{
			Uint8 b, g, r, a;
		};
	};
};

inline Color operator* ( Float v, Color col )
{
	Color result;
	result.a = static_cast< Uint8 >( col.a * v );
	result.r = static_cast< Uint8 >( col.r * v );
	result.g = static_cast< Uint8 >( col.g * v );
	result.b = static_cast< Uint8 >( col.b * v );
	return result;
}

inline Color operator+ ( Color a, Color b )
{
	Color result;

	result.a = ( 0xff - a.a < b.a ) ? 0xff : ( a.a + b.a );
	result.r = ( 0xff - a.r < b.r ) ? 0xff : ( a.r + b.r );
	result.g = ( 0xff - a.g < b.g ) ? 0xff : ( a.g + b.g );
	result.b = ( 0xff - a.b < b.b ) ? 0xff : ( a.b + b.b );

	return result;
}