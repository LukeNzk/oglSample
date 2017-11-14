#pragma once
#include "types.h"

#include <string>
#include <memory>

namespace sc
{
	class AbsolutePath
	{
	public:
		AbsolutePath( const char* path );
		~AbsolutePath();

		Bool IsDirectory() const;
		const char* GetFileName() const;
		void AppendDirectory( const char* dirName );
		void SetFile( const char* fileName );
		const AnsiChar* Get() const { return m_path.c_str(); };
		void DirUp();

	private:
		void Reparse();

	private:
		std::string		m_path;
		Uint32			m_fileNameIndex;
	};
}