#include "test_common.h"
#include "../td4_decoder.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_out_im(void)
{
	struct td4_state *state = init_state();
	bool b;

	// OUT Im ;  1011 : 0001 0xb1
	set_carry_flag(state, 1);
	parse_opecode(state, 0xb1);
	b = (state->io->out_port == 0x01) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT_IM Test001");

	// OUT Im ; 1011 : 0000 
	set_carry_flag(state, 1);
	parse_opecode(state, 0xb0);
	b = (state->io->out_port == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT IM Test002");

	// OUT Im ; 1011 : 1111 
	set_carry_flag(state, 1);
	parse_opecode(state, 0xBF);
	b = (state->io->out_port == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT IM Test003");

	cleanup_state(state);

	return 0;
}

int test_out_b(void)
{
	struct td4_state *state = init_state();
	bool b;

	// OUT Im ;  1011 : 0001 0xb1
	set_carry_flag(state, 1);
	state->acc->reg_b = 0x01;
	parse_opecode(state, 0x90);
	b = (state->io->out_port == 0x01) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT_B Test001");

	// OUT Im ; 1011 : 0000 
	set_carry_flag(state, 1);
	state->acc->reg_b = 0x00;
	parse_opecode(state, 0x90);
	b = (state->io->out_port == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT B Test002");

	// OUT Im ; 1011 : 1111 
	set_carry_flag(state, 1);
	state->acc->reg_b = 0x0f;
	parse_opecode(state, 0x90);
	b = (state->io->out_port == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "OUT B Test003");

	cleanup_state(state);

	return 0;
}


int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_out_im();
	ret = test_out_b();

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

