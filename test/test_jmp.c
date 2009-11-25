#include "test_common.h"
#include "../td4_decoder.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_jmp(void)
{
	struct td4_state *state = init_state();
	bool b;

	// jmp 0x03 ; 1111 : 0011 
	state->ip = 0;
	parse_opecode(state, 0xf3);
	b = (state->ip == 0x03) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "JMP Test001");

	// jmp 0x00 ; 1111 : 0000 
	state->ip = 0;
	parse_opecode(state, 0xf0);
	b = (state->ip == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "JMP Test002");

	// jmp 0x0f ; 1111 : 1111 
	state->ip = 0;
	parse_opecode(state, 0xff);
	b = (state->ip == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "JMP Test003");

	cleanup_state(state);

	return 0;
}

int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_jmp();

	printf("Total test case is %d\n", get_total_test_count());
	printf("Total passed test case is %d\n", get_total_pass_count());
	printf("Total failed test case is %d\n", get_total_fail_count());
	cleanup_opcode_table();

	return ret;
}

int main(int argc, char **argv)
{
	return start_test();
}

