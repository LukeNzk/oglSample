#pragma once
#include "../../utils/public/types.h"

class ImageBuffer
{
public:
	virtual void WriteColor( Uint32 x, Uint32 y, Uint32 color ) = 0;
	virtual void WriteColor( Uint32 i, Uint32 color ) = 0;

	virtual void WriteDepth( Uint32 x, Uint32 y, Float depth ) = 0;

	virtual Float GetDepth( Uint32 x, Uint32 y ) const = 0;
	virtual void* GetColorData() const = 0;

	virtual void ClearDepth() = 0;
	virtual void Clear( Uint32 color ) = 0;

	inline Uint32 DataSize() const { return m_width * m_height * sizeof( Uint32 ); }
	inline Uint32 Size() const { return m_width * m_height; }
	inline float GetAspectRatio() const { return m_ar; }
	inline Uint32 Width() const { return m_width; }
	inline Uint32 Height() const { return m_height; }

	virtual inline void SetSize( Uint32 width, Uint32 height )
	{
		m_width = width;
		m_height = height;
		m_ar = ( Float )width / height;
	}

protected:
	Uint32	m_width;
	Uint32	m_height;
	Float	m_ar;
};