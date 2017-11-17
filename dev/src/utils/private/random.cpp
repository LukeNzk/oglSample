#include "random.h"
#include <random>

#define HALF_RAND_MAX 0x3FFF

void RandomSeed( Int32 seed )
{
	srand( seed );
}

Float Random01f()
{
	Float r = static_cast < Float > ( rand() ) / static_cast <float> ( RAND_MAX );
	return r;
}

Float RandomSign()
{
	if ( rand() < HALF_RAND_MAX )
		return -1.0f;
	else return 1.0f;
}