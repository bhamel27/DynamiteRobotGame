#include <main.h>
#include "tp1.h"

int main(int argc, const char* argv[])
{
	// First argument is log file, if it exists
	if (argc >= 1)
		Log::SetFile(argv[0]);

	CoreTP1 core;

	core.Run();

	return 0;
}