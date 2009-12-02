#include "xmalloc.h"
#include "td4emu.h"

void *xmalloc(size_t size)
{
	void *ret;
	if (size <= 0)
		td4_abort("allocate size must be more than 0");

	ret = malloc(size);
	if (!ret)
		td4_abort("cannot allocate memory");

	return ret;
}
