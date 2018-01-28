#include "build.h"
#include "testFramework.h"

int main( int argc, char** argv )
{	
	testing::InitGoogleTest( &argc, argv );
	const int errorCode = RUN_ALL_TESTS();
	return errorCode;
}