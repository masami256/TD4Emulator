#ifndef XMALLOC_H
#define XMALLOC_H 1

#include <stdlib.h>

void *xmalloc(size_t size);

#define xfree(ptr) { \
	free(ptr); \
	ptr = NULL; \
}

#endif // XMALLOC_H
