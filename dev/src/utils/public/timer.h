#pragma once
#include "types.h"

class Timer
{
public:
	Timer( bool jumpStart = false );

	void Start();

	Double TimeElapsed() const;

private:
	Double m_start;
};
