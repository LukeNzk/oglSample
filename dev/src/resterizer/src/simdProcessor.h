#pragma once
#include "../../utils/public/types.h"

#include <xmmintrin.h>

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

__declspec( align( 16 ) )
struct QColor
{
	union
	{
		struct
		{
			__m128 r;
			__m128 g;
			__m128 b;
			__m128 a;
		} mm;

		struct
		{
			Float r[ 4 ];
			Float g[ 4 ];
			Float b[ 4 ];
			Float a[ 4 ];
		};
	};

	void Set( Uint32 index, const Float* val )
	{
		r[ index ] = val[ 0 ];
		g[ index ] = val[ 1 ];
		b[ index ] = val[ 2 ];
		a[ index ] = val[ 3 ];
	}
};

class SIMDProcessor
{
public:
	void Process()
	{
		ComputeEdges();
		TestEdges();
		BccMulInvArea();
		CalcDepth();
		CalcPixelPos();
		CalcColor();
	}

private:
	void ComputeEdges();
	void TestEdges();
	void BccMulInvArea();
	void CalcDepth();
	void CalcPixelPos();
	void CalcColor();

public:
	QColor m_vColors[ 3 ];
	QColor m_color;
	QVec3 m_ssVerts[ 3 ];
	QVec3 m_wsVerts[ 3 ];
	QVec3 m_wsPx;

	QVec3 m_pixelPos;
	QVec3 m_bcc; // barycentric
	QVec3 m_pixelCol;

	QFloat m_oneOverZ;
	QFloat m_invArea;
};
