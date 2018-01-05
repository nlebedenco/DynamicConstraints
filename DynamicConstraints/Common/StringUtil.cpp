#include "stringutil.h"

#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

std::string
std::format(const char* fmt, ...)
{
	std::string result;
	if (fmt)
	{
		va_list ap;
		va_start(ap, fmt);
		size_t n;

		try
		{
			// first pass, find the buffer size needed for the parsing
			n = vsnprintf(0, 0, fmt, ap) + 1; // number of generated characters plus NULL termination
			if (n <= 0)
				throw std::runtime_error("Error parsing string format: " + std::string(strerror(errno)));
		}
		catch (...)
		{
			va_end(ap);
			throw;
		}

		// Now we can allocate a buffer of the right length
		char* text = new char[n];
		try
		{
			memset(text, 0, n);					// initialize
			n = vsnprintf(text, n, fmt, ap);	// perform the formatting
			if (n <= 0)
				throw std::runtime_error("Error parsing string format: " + std::string(strerror(errno)));
			result = text;
		}
		catch (...)
		{
			va_end(ap);
			delete[] text;
			throw;
		}
		va_end(ap);
		delete[] text;
	}
	return result;
}

