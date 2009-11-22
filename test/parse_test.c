#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../td4emu.h"
#include "../td4_opcode.h"
#include "../state_machine.h"

void print_result(bool b, char *msg)
{
	if (b)
		printf("\x1b[1;32m%s", msg);
	else
		printf("\x1b[1;31m%s", msg);
	printf("\x1b[0m");

}

int test_parse_opcode(void)
{
	struct td4_state *state = init_state();

	// 0x03: 0000 0011
	// Add 0x03 to register A
	parse_opecode(state, 0x03);

	if (state->acc->reg_a == 3)
		print_result(true, "Test001 OK\n");
	else
		print_result(false, "Test001 NG\n");

	// 0x76: 0111 0110 
	// MOV 0x06 to register B
	parse_opecode(state, 0x76);
	if (state->acc->reg_b == 6)
		print_result(true, "Test002 OK\n");
	else
		print_result(false, "Test002 NG\n");


	return 0;
	
}

int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_parse_opcode();

	cleanup_opcode_table();

	return ret;
}

int main(int argc, char **argv)
{
	return start_test();
}

