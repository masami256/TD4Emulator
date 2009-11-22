#include <stdio.h>
#include <stdlib.h>

void td4_abort(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	abort();
}
