#pragma once

#include "debug.h"

#define SC_INLINE inline
#define SC_FORCEINLINE __forceinline

#if defined( DEBUG ) || defined( _DEBUG )
#define SC_BREAK() __debugbreak()
#define SC_ASSERT( cond, format, ... ) if ( !( cond ) ) { Debug::SCDebug::AssertWindow( format, __VA_ARGS__ ); __debugbreak(); }
#define SC_VALIDATE( expr ) expr
#else
#define SC_BREAK()
#define SC_ASSERT( cond, format, ... )
#define SC_VALIDATE( expr )
#endif