#include "Application.h"
#include <cstdlib>

void pause()
{
	fprintf(stderr, "Program terminated.\n\n");
	system("pause");
}

int main(int argc, char* argv[])
{
	// atexit(pause);

	Application app("Soft Geometry Simulator");
	return app.Run();
}

// For compatibility with SDL libs
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
