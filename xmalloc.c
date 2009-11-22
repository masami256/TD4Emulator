#include "xmalloc.h"
#include "td4emu.h"

void *xmalloc(size_t size)
{
	if (size <= 0)
		td4_abort("cannot allocate memory");

	return malloc(size);
}
