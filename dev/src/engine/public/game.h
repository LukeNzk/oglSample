#pragma once
#include "../../utils/public/types.h"

class IGame
{
public:
	IGame() { }
	virtual ~IGame() { }

	virtual void Tick( Float dt ) = 0;
};