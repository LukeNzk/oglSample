#pragma once
#include "build.h"
#include "../../../../external/googletest/include/gtest/gtest.h"

#include "../../../utils/public/fileStream.h"
#include "../../../utils/public/fileUtils.h"
#include "../../../utils/public/macros.h"

TEST( FileUtils, CreateRemove )
{
	std::string path = sc::GetExecutableDir();
	path += "testFile.txt";

	{
		sc::FileStream fs = *sc::FileStream::Create( path.c_str() );
	}

	ASSERT_TRUE( sc::FileExists( path.c_str() ) );

	sc::RemoveFile( path.c_str() );
	ASSERT_FALSE( sc::FileExists( path.c_str() ) );
}

TEST( DirectoryUtils, CreateRemove )
{
	std::string path = sc::GetExecutableDir();
	path += "testDir/";

	sc::CreateDir( path.c_str() );
	EXPECT_TRUE( sc::DirectoryExists( path.c_str() ) );

	sc::RemoveDir( path.c_str() );
	EXPECT_FALSE( sc::DirectoryExists( path.c_str() ) );
}

TEST( FileStream, CreateFile )
{
	std::string path = sc::GetExecutableDir();
	path += "testFile.txt";

	// create and write
	{
		sc::FileStream fs = *sc::FileStream::Create( path.c_str() );
		ASSERT_TRUE( sc::FileExists( path.c_str() ) );

		const char* content = "dupa";
		const Uint32 len = ( Uint32 )strlen( content ) + 1;
		fs.Write( content, len );
	}

	{ // check file size
		sc::FileStream fs = *sc::FileStream::Create( path.c_str() );

		ASSERT_EQ( 5, fs.GetSize() );
	}

	sc::RemoveFile( path.c_str() );
}

//////////////////////////////////////////////////////////////////////////
// FileStreamTest
//////////////////////////////////////////////////////////////////////////
class FileStreamTest : public ::testing::Test
{
protected:
	static void SetUpTestCase()
	{
		s_path = sc::GetExecutableDir();
		s_path += "testFile.txt";
	}

	static void TearDownTestCase()
	{
	}

	virtual void SetUp() override
	{
		sc::FileStream fs = *sc::FileStream::Create( s_path.c_str() );
		SC_ASSERT( sc::FileExists( s_path.c_str() ), "Failed to create test file" );

		const char* content = "test";
		const Uint32 len = ( Uint32 )strlen( content ) + 1;
		fs.Write( content, len );
	}

	virtual void TearDown() override
	{
		sc::RemoveFile( s_path.c_str() );
		SC_ASSERT( !sc::FileExists( s_path.c_str() ), "Failed to delete test file" );
	}

	const std::string& GetPath() const { return s_path; }

private:
	static std::string	s_path;
};

std::string FileStreamTest::s_path = "";

TEST_F( FileStreamTest, ReadWrite )
{
	sc::FileStream* fs = sc::FileStream::Open( GetPath().c_str() );
	EXPECT_NE( nullptr, fs );

	char buff[ 10 ];
	fs->Read( buff, 5 );

	EXPECT_EQ( 5, fs->GetOffset() );
	EXPECT_EQ( 0, strcmp( "tes", buff ) );

	delete fs;
}

TEST_F( FileStreamTest, Offset )
{
	sc::FileStream* fs = sc::FileStream::Open( GetPath().c_str() );
	EXPECT_NE( nullptr, fs );

	char buff[ 10 ];
	fs->SetOffset( 2 );
	fs->Read( buff, 3 );

	EXPECT_EQ( 5, fs->GetOffset() );
	EXPECT_EQ( 0, strcmp( "st", buff ) );

	delete fs;
}