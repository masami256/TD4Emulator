#include "test_common.h"
#include "../td4_opcode.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_add_a(struct td4_state *state)
{
	bool b;

	// 0x03: 0000 0011
	// Add 0x03 to register A
	parse_opecode(state, 0x03);
	b = (state->acc->reg_a == 0x03) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
		
	print_result(b, "ADD_A Test001");

	// 0x03 + 0x0c = 0x0f
	parse_opecode(state, 0xc);
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_A Test002");

	// 0x0f + 0x01 = 0x10. but TD4 use 4bit register so it should be 0x0.
	// and carry flag should 1.
	parse_opecode(state, 0x1);
	b = (state->acc->reg_a == 0x00) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 1)
			b = false;
	}
	print_result(b, "ADD_A Test003");

	// when carry flag is 1, it shold be re-set next time. 
	parse_opecode(state, 0xf);
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_A Test004");

	// 0x0f + 0x0f = 0x0e and carry flag shold be 1.
	parse_opecode(state, 0xf);
	b = (state->acc->reg_a == 0x0e) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 1)
			b = false;
	}
	print_result(b, "ADD_A Test005");

	// 0x0e + 0x0 = 0 and carry flag shold be 0.
	parse_opecode(state, 0x0);
	b = (state->acc->reg_a == 0x0e) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_A Test006");

	return 0;
}

int test_add_b(struct td4_state *state)
{
	bool b;

	// 0x53: 0101 0011
	// Add 0x03 to register B
	parse_opecode(state, 0x53);
	b = (state->acc->reg_b == 0x03) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_B Test001");

	// 0x03 + 0x0c = 0x0f
	// 0101 1100 = 0x5c
	parse_opecode(state, 0x5c);
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_B Test002");

	// 0x0f + 0x01 = 0x10. but TD4 use 4bit register so it should be 0x0.
	// and carry flag should 1.
	// 0101 0001 = 0x51
	parse_opecode(state, 0x51);
	b = (state->acc->reg_b == 0) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 1)
			b = false;
	}
	print_result(b, "ADD_B Test003");

	// when carry flag is 1, it shold be re-set next time. 
	// 0101 1111 = 0x5f
	parse_opecode(state, 0x5f);
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_B Test004");

	// 0x0f + 0x0f = 0x0e and carry flag shold be 1.
	// 0101 1111 = 0x5f
	parse_opecode(state, 0x5f);
	b = (state->acc->reg_b == 0x0e) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 1)
			b = false;
	}
	print_result(b, "ADD_B Test005");

	// 0x0e + 0x0 = 0 and carry flag shold be 0.
	// 01010000
	parse_opecode(state, 0x50);
	b = (state->acc->reg_b == 0x0e) ? true : false;
	if(b) {
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "ADD_B Test006");

	return 0;
}

int test_add(void)
{
	struct td4_state *state = init_state();

	test_add_a(state);
	test_add_b(state);

	cleanup_state(state);

	return 0;
	
}

int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_add();

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

