#pragma once
#include "types.h"

template< typename T >
class Timer
{
public:
	Timer();

	void Start();
	T TimeElapsed() const;

private:
	T m_start;
};