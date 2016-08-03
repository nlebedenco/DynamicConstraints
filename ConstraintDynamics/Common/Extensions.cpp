#include "Extensions.h"
#include "StringUtil.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cfloat>   // DBL_MAX
#include <cmath>    // std::nextafter
#include <algorithm>

std::default_random_engine&
std::
global_urng()
{
	static std::default_random_engine u{};
	return u;
}

void
std::
randomize()
{
	static std::random_device rd{};
	global_urng().seed(rd());
}


double
std::
random()
{
    // Note: uniform_real_distribution does [start, stop), but we want to do [start, stop].
    // Pass the next largest value instead.
    static std::uniform_real_distribution<double> next(0, std::nextafter(1, DBL_MAX));
    return next(std::global_urng());
}

double
std::
random(double min, double max)
{
    return min + (random() * (max - min));
}

void
std::
pexit(int code, const char* fmt, ...)
{
	if (fmt)
	{
		va_list ap;
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
	}
	exit(code);
}