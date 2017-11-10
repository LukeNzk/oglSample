#pragma once
#include "stream.h"
#include <string>

namespace sc
{
	class FileStream final : public IStream
	{
	public:
		~FileStream();

		// write to file (data will appear on disk after closing the stream)
		void Write( const void* data, Uint32 size ) override;

		// read file content to a buffer
		void Read( void* buffer, Uint32 size ) override;

		// set read/write offset in bytes
		void SetOffset( Uint32 pos ) override;

		// get read/write offset
		Uint32 GetOffset() const override;

		// read size of the file
		Uint32 GetSize() const override;

		// open existing file
		static FileStream* Open( const char* filename );

		// create new file
		static FileStream* Create( const char* filename );

	private:
		FileStream( void* file );

	private:
		std::string		m_filename;	//!< file name
		void*			m_file;		//!< native file handle
		Uint32			m_off;		//!< offset
		Uint32			m_size;		//!< size
	};
}