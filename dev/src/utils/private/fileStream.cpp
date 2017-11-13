#include "types.h"
#include "macros.h"
#include "fileStream.h"

#include <windows.h>

namespace sc
{
	FileStream::FileStream( void* file )
		: m_file( file )
		, m_off( 0 )
	{
	}
	
	FileStream::~FileStream()
	{
		CloseHandle( ( HANDLE )m_file );
		m_file = nullptr;
	}

	void FileStream::Write( const void* data, Uint32 size )
	{
		SC_ASSERT( m_file, "" );
		
		DWORD bytesWritten = 0;
		const BOOL errorFlag = WriteFile(
			( HANDLE )m_file, // file handle
			data, // data pointer
			size, // data size
			&bytesWritten,
			NULL // overlapped
			);

		SC_ASSERT( errorFlag, "Failed to write file." );
		SC_ASSERT( ( Uint32 )bytesWritten == size, "Incomplete write." );

		m_off += size;
	}

	void FileStream::Read( void* buffer, Uint32 size )
	{
		SC_ASSERT( m_file, "" );
		DWORD bytesRead;
		const BOOL errorFlag = ReadFile(
			m_file,
			buffer,
			size,
			&bytesRead,
			NULL
			);

		SC_ASSERT( errorFlag, "Failed to read file." );
		SC_ASSERT( ( Uint32 )bytesRead == size, "Incomplete read." );

		m_off += size;
	}

	void FileStream::SetOffset( Uint32 pos )
	{
		const Int32 diff = pos - m_off;
		const DWORD err = SetFilePointer( ( HANDLE )m_file, diff, NULL, FILE_CURRENT );
		
		SC_ASSERT( err != INVALID_SET_FILE_POINTER, "Failed to set file pointer." );

		m_off = pos;
	}

	Uint32 FileStream::GetOffset() const
	{
		return m_off;
	}

	Uint32 FileStream::GetSize() const
	{
		const DWORD result = GetFileSize( ( HANDLE )m_file, NULL );

		SC_ASSERT( result != INVALID_FILE_SIZE, "Could not read file size." );

		// TODO: check the file and eventually revise this class to handle such a big thing
		SC_ASSERT( result <= UINT32_MAX, "File bigger than 2^32 B (4.3 GB)" );

		return ( Uint32 )result;
	}

	FileStream* FileStream::Open( const char* path )
	{
		HANDLE file = CreateFile(
			path,			// name
			GENERIC_WRITE | GENERIC_READ,	// access mode
			FILE_SHARE_READ,// file share
			NULL,			// security attribute
			OPEN_EXISTING,	// open | create
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		SC_ASSERT( file != INVALID_HANDLE_VALUE, "Could not open file %s.", path );
		
		if ( file )
		{
			FileStream* fs = new FileStream( file );
			fs->m_filename = std::string( path );
			return new FileStream( file );
		}
			
		return nullptr;
	}

	FileStream* FileStream::Create( const char* path )
	{
		HANDLE file = CreateFile(
			path,			// name
			GENERIC_WRITE,	// access mode
			FILE_SHARE_READ,// file share
			NULL,			// security attribute
			OPEN_ALWAYS,	// open | create
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		SC_ASSERT( file != INVALID_HANDLE_VALUE, "Could not open file %s.", path );

		if ( file )
		{
			FileStream* fs = new FileStream( file );
			fs->m_filename = std::string( path );
			return new FileStream( file );
		}
			
		return nullptr;
	}
}