#include "xmalloc.h"

void *xmalloc(size_t size)
{
	if (size <= 0)
		return NULL;

	return malloc(size);
}
