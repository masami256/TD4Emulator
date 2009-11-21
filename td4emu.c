#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "td4emu.h"

// TD4's memory space.
unsigned char memory[ADDRESS_SPACE_SIZE];

// TD4's program counter
unsigned int IP;

int main(int argc, char **argv)
{
	struct td4_registers regs;

	memset(&regs, 0, sizeof(regs));

	init_opcode_table();

	cleanup_opcode_table();
	
	return 0;
}
