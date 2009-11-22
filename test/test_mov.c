#include "test_common.h"
#include "../td4_opcode.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_mov_a_im(struct td4_state *state)
{
	bool b;

	// mov a, 0x1 ; 0011 0001  = 0x31
	parse_opecode(state, 0x31);
	b = (state->acc->reg_a == 0x01) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A_IM Test001");		

	// mov a, 0x0f ; 0011 1111  = 0x3f
	parse_opecode(state, 0x3f);
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A_IM Test002");		

	// mov a, 0x0 ; 0011 0000  = 0x30
	parse_opecode(state, 0x30);
	b = (state->acc->reg_a == 0x0) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A_IM Test003");		

	return 0;
}

int test_mov_b_im(struct td4_state *state)
{
	bool b;
	
	// mov b, 0x1 ; 0111 0001  = 0x71
	parse_opecode(state, 0x71);
	b = (state->acc->reg_b == 0x01) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B_IM Test001");		

	// mov b, 0x0f ; 0111 1111  = 0x7f
	parse_opecode(state, 0x7f);
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B_IM Test002");		

	// mov b, 0x0 ; 0111 0000  = 0x70
	parse_opecode(state, 0x70);
	b = (state->acc->reg_b == 0x0) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B_IM Test003");		
	return 0;
}

int test_mov_a2b(struct td4_state *state)
{
	bool b;

	// A register : 1111 to B register
	state->acc->reg_a = 0x0f;
	state->acc->reg_b = 0x02; // dummy data.
	parse_opecode(state, 0x40);
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A2B Test001");		

	// A register : 0000 to B register
	state->acc->reg_a = 0x00;
	state->acc->reg_b = 0x02; // dummy data.
	parse_opecode(state, 0x40);
	b = (state->acc->reg_b == 0x00) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A2B Test002");		

	// A register : 0010 to B register
	state->acc->reg_a = 0x02;
	state->acc->reg_b = 0x02; // dummy data.
	parse_opecode(state, 0x40);
	b = (state->acc->reg_b == 0x02) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_A2B Test003");		

	return 0;
}

int test_mov_b2a(struct td4_state *state)
{
	bool b;

	// B register : 1111 to A register
	state->acc->reg_a = 0x02; // dummy data.
	state->acc->reg_b = 0x0f; 
	parse_opecode(state, 0x10);
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B2A Test001");		

	// B register : 0000 to A register
	state->acc->reg_a = 0x02; // dummy data.
	state->acc->reg_b = 0x00; 
	parse_opecode(state, 0x10);
	b = (state->acc->reg_a == 0x00) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B2A Test002");		

	// B register : 0010 to A register
	state->acc->reg_a = 0x02; // dummy data.
	state->acc->reg_b = 0x02; 
	parse_opecode(state, 0x10);
	b = (state->acc->reg_a == 0x02) ? true : false;
	if (b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "MOV_B2A Test003");		

	return 0;
}

int test_mov(void)
{
	struct td4_state *state = init_state();

	test_mov_a_im(state);
	test_mov_b_im(state);
	
	state = reset_state(state);
	test_mov_a2b(state);
	test_mov_b2a(state);

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

