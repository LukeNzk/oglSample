#pragma once
#include "types.h"

//#define NO_LOGGING
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NO_LOGGING)
#define DEBUG_LEVEL 0 // 0 - break at info, 4 - never break
#include <iostream>
#include <stdio.h>

namespace Debug
{
	extern void ShowMessageBox( const AnsiChar* const, const AnsiChar* const );
	extern void ShowErrorMessage( const AnsiChar* const, const AnsiChar* const );

	//Use macros instead of it
	class SCDebug
	{
	public:
		template<typename... Args>
		static void Info( char* messageFormat, Args... args )
		{
			std::printf( messageFormat, args... );
		}

		template<typename... Args>
		static void Warning( const AnsiChar* messageFormat, Args... args )
		{
			std::printf( "Warning: " );
			std::printf( messageFormat, args... );

			if ( DEBUG_LEVEL < 2 )
			{
				__debugbreak();
			}
		}

		template<typename... Args>
		static void Error( const AnsiChar* messageFormat, Args... args )
		{
			std::printf( "Error: " );
			std::printf( messageFormat, args... );
			if ( DEBUG_LEVEL < 3 )
			{
				__debugbreak();
			}
		}

		template<typename... Args>
		static void AssertWindow( const AnsiChar* messageFormat, Args... args )
		{
			const static AnsiChar* const assertTitle = "Assertion failed";
			AnsiChar msg[ 512 ];
			
			sprintf_s( msg, 512, messageFormat, args... );
			
			ShowErrorMessage( msg, assertTitle );
			std::printf( messageFormat, args... );
		}
	};
}

#define BREAK_AT_LEVEL(x) if ( DEBUG_LEVEL <= x ) GF_BREAK()

#define DEBUG_INFO(...) { Debug::SCDebug::Info( __VA_ARGS__ ); BREAK_AT_LEVEL(0); }
#define DEBUG_WARNING(...) { Debug::SCDebug::Warning( __VA_ARGS__ ); BREAK_AT_LEVEL(1); }
#define DEBUG_ERROR(...) { Debug::SCDebug::Error( __VA_ARGS__ ); BREAK_AT_LEVEL(2); }
#define DEBUG_FATAL(...) { Debug::SCDebug::Fatal( __VA_ARGS__ ); BREAK_AT_LEVEL(3); }

#else

#define DEBUG_INFO(x)
#define DEBUG_WARNING(x)
#define DEBUG_ERROR(x)
#define DEBUG_FATAL(x)

#endif