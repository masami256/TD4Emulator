#include "test_common.h"
#include "../td4_decoder.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_in_a(void)
{
	struct td4_state *state = init_state();
	bool b;

	// IN A ; 0010 : 0000 
	// in port is 0x0f
	set_carry_flag(state, 1);
	state->io->in_port = 0x0f;
	parse_opecode(state, 0x20);
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_A Test001");

	// IN A ; 0010 : 0000 
	// in port is 0x00
	set_carry_flag(state, 1);
	state->io->in_port = 0x00;
	parse_opecode(state, 0x20);
	b = (state->acc->reg_a == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_A Test002");

	// IN A ; 0010 : 0000 
	// in port is 0x01
	set_carry_flag(state, 1);
	state->io->in_port = 0x01;
	parse_opecode(state, 0x20);
	b = (state->acc->reg_a == 0x01) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_A Test003");

	cleanup_state(state);

	return 0;
}

int test_in_b(void)
{
	struct td4_state *state = init_state();
	bool b;

	// IN B ; 0110 : 0000 
	// in port is 0x0f
	set_carry_flag(state, 1);
	state->io->in_port = 0x0f;
	parse_opecode(state, 0x60);
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_B Test001");

	// IN B ; 0110 : 0000 
	// in port is 0x00
	set_carry_flag(state, 1);
	state->io->in_port = 0x00;
	parse_opecode(state, 0x60);
	b = (state->acc->reg_b == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_B Test002");

	// IN B ; 0110 : 0000 
	// in port is 0x01
	set_carry_flag(state, 1);
	state->io->in_port = 0x01;
	parse_opecode(state, 0x60);
	b = (state->acc->reg_b == 0x01) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "IN_B Test003");

	cleanup_state(state);

	return 0;
}


int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_in_a();
	ret = test_in_b();

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

