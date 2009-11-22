#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "td4emu.h"
#include "state_machine.h"

int start_emulation(char *file)
{
	struct td4_state *state;

	// initializing registers and others.
	state = init_state();
	init_opcode_table();
	
	cleanup_opcode_table();
	
	return 0;
}

