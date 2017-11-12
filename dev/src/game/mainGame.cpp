#include "..\..\engine\public\renderer.h"
#include <stdlib.h>
#include "..\..\utils\public\macros.h"

void main()
{
	SC_ASSERT( false, "%s dupa %d", "asdf", 15 );
	Renderer rend;
	rend.Draw();
	system( "pause" );
}