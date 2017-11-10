#pragma once
#include "types.h"

namespace sc
{
	class IStream
	{
	public:
		virtual ~IStream() { }

		virtual void Write( const void* data, Uint32 size ) = 0;
		virtual void Read( void* data, Uint32 size ) = 0;
		
		virtual void SetOffset( Uint32 pos ) = 0;
		virtual Uint32 GetOffset() const = 0;

		virtual Uint32 GetSize() const = 0;
	};
}