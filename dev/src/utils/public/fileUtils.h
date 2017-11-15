#pragma once
#include <string>

namespace sc
{
	namespace path
	{
		const Uint32 FileNameIndex( const AnsiChar* path );

		Bool IsDirectory( const char* path );

		const char* GetExtension( const char* path );
	}

	// check if file exists
	Bool FileExists( const char* path );

	// remove file from disk
	Bool RemoveFile( const char* name );

	// creat new directory
	Bool CreateDir( const char* path );

	// check if directory exists
	Bool DirectoryExists( const char* path );

	// remove directory
	Bool RemoveDir( const char* path );

	// full, absolute path of current executable
	std::string GetExecutablePath();

	// directory of current executable
	std::string GetExecutableDir();

	std::string GetResourcePath( const AnsiChar* relativePath );

	std::string GetResourcesDir();
}