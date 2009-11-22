#include "test_common.h"
#include "../td4_opcode.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_mov_a(struct td4_state *state)
{
	return 0;
}

int test_mov_b(struct td4_state *state)
{
	bool b;

	// 0x76: 0111 0110 
	// MOV 0x06 to register B
	parse_opecode(state, 0x76);
	b = (state->acc->reg_b == 6) ? true : false;
	print_result(b, "MOV Test001");

	return 0;
}

int test_mov(void)
{
	struct td4_state *state = init_state();

	test_mov_a(state);
	test_mov_b(state);

	cleanup_state(state);

	return 0;
	
}


int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_mov();

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

