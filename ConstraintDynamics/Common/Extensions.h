#pragma once

#include <random>

namespace std
{
	std::default_random_engine&
	global_urng();
	
	void
	randomize();

    double
    random();

    double
    random(double min, double max);

	template<typename T>
    const T&
	clamp(const T& value, const T&  min, const T&  max)	{ return std::min(std::max(min, value), max); }

	void
	pexit(int code, const char* fmt, ...);
}



