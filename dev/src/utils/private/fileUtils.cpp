#include "types.h"
#include "macros.h"
#include "fileUtils.h"
#include "absolutePath.h"

#include <windows.h>

namespace sc
{
	namespace path
	{
		Bool path::IsDirectory( const char* path )
		{
			const Uint32 pathLen = static_cast< Uint32 >( strlen( path ) );
			return path[ pathLen - 1 ] == '\\' || path[ pathLen - 1 ] == '/';
		}

		const char* GetExtension( const char* path )
		{
			if ( IsDirectory( path ) )
				return nullptr;

			const Uint32 len = static_cast< Uint32 >( strlen( path ) );

			const char* cIter = path + len;
			while ( cIter != path )
			{
				if ( *cIter == '/' || *cIter == '\\' )
				{
					// no extension
					return nullptr;
				}

				if ( *cIter == '.' )
				{
					++cIter;
					break;
				}

				--cIter;
			}

			return cIter;
		}

	}

	Bool FileExists( const char* path )
	{
		DWORD dwAttrib = GetFileAttributes( path );

		if ( dwAttrib == INVALID_FILE_ATTRIBUTES )
			return false;

		if ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY )
			return false; // file is a directory

		return true;

	}

	Bool RemoveFile( const char* path )
	{
		const Bool success = ::DeleteFile( path ) == TRUE;

		if ( !success )
		{
			const DWORD err = GetLastError();
			SC_ASSERT( err == ERROR_FILE_NOT_FOUND, "File not found ['%s']", path );
			SC_ASSERT( err == ERROR_ACCESS_DENIED, "File access denied ['%s']", path );
		}

		return success;
	}

	Bool CreateDir( const char* path )
	{
		SC_ASSERT( path::IsDirectory( path ), "Not a directory path." );

		const BOOL created = ::CreateDirectory( path, NULL );

		if ( !created )
		{
			const DWORD err = GetLastError();
			SC_ASSERT( err == ERROR_ALREADY_EXISTS, "Directory already exists." );
			SC_ASSERT( err == ERROR_PATH_NOT_FOUND, "Invalid path." );
			return false;
		}

		return true;
	}

	Bool DirectoryExists( const char* path )
	{
		DWORD dwAttrib = GetFileAttributes( path );

		if ( dwAttrib == INVALID_FILE_ATTRIBUTES )
			return false;

		return ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) ? true : false;
	}

	Bool RemoveDir( const char* path )
	{
		SC_ASSERT( path::IsDirectory( path ), "Not a directory path." );
		const BOOL removed = ::RemoveDirectory( path );

		if ( !removed )
		{
			return false;
		}

		return true;
	}

	std::string GetExecutablePath()
	{
		const Uint32 sz = 500;
		char buffer[ sz ];
		const DWORD nSize = GetModuleFileName( GetModuleHandle( 0 ), buffer, sz );

		SC_ASSERT( nSize < sz, "Path is longer than a buffer" );
		return std::string( buffer );
	}

	std::string GetExecutableDir()
	{
		std::string result = GetExecutablePath();
		Uint32 endPos = static_cast< Uint32 >( result.size() );
		SC_ASSERT( endPos > 0, "Empty string" );

		const char* ptr = result.c_str();
		while ( endPos > 0 )
		{
			if ( ptr[ endPos ] == '\\' )
				break;
			--endPos;
		}
		SC_ASSERT( ptr[ endPos ] == '\\', "Can't extract directory of the executable." );

		return result.substr( 0, endPos + 1 );
	}

	std::string GetResourcesDir()
	{
		sc::AbsolutePath path( GetExecutableDir().c_str() );
		path.DirUp();
		path.DirUp();

		path.AppendDirectory( "assets" );
		return path.Get();
	}
}