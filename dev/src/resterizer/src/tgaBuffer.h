#pragma once
#include "imageBuffer.h"

#include "../../utils/public/types.h"

class TgaBuffer : public ImageBuffer
{
public:
	TgaBuffer(Uint32 width, Uint32 height);
	~TgaBuffer();

	void WriteColor(Uint32 x, Uint32 y, Uint32 color) override;
	void WriteColor(Uint32 i, Uint32 color) override;

	void WriteDepth( Uint32 x, Uint32 y, Float depth ) override;

	Float GetDepth( Uint32 x, Uint32 y ) const override { return m_depth[ x + y * m_width ]; }
	void* GetColorData() const override { return m_color; }

	void ClearDepth();
	void Clear( Uint32 color );

	void Save( const char* fileName );

private:
	Uint32* m_color;
	Float* m_depth;
};