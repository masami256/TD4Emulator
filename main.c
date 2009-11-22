#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "td4emu.h"


void usage(char *prog)
{
	fprintf(stderr, "usage %s file\n", prog);
	exit(-1);
}

int main(int argc, char **argv)
{
	if (argc != 2) 
		usage(argv[0]);

	return start_emulation(argv[1]);
}

