#include "test_common.h"
#include "../td4_opcode.h"
#include "../state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_decoder_add_a(struct td4_state *state)
{
	bool b;
	int i;

	state = reset_state(state);

	// Add 0x01 to regster A 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x01;

	decoder(state);

	// register A should be 0x0f.
	b = (state->acc->reg_a == 0x0f) ? true : false;
	if(b) { 
		// last execution is "add A, 0".
		// so, carry should be 0.
		if (get_carry_flag(state) != 0)
			b = false;
	}		
	print_result(b, "DECODER ADD_A Test001");

	state = reset_state(state);

	// Add 0x01 to regster A 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x01;
	// then add 0x06.
	state->memory[ADDRESS_SPACE_SIZE - 1] = 0x06;

	decoder(state);

	// register A should be 0x5.
	b = (state->acc->reg_a == 0x5) ? true : false;
	if(b) { 
		// 0x0f + 0x06 will overflow 4bit range.
		if (get_carry_flag(state) != 1)
			b = false;
	}		
	print_result(b, "DECODER ADD_A Test002");

	state = reset_state(state);

	// Add 0x01 to regster A 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x01;
	// then add 0x0f.
	state->memory[ADDRESS_SPACE_SIZE - 1] = 0x0f;

	decoder(state);

	// register A should be 0xe.
	b = (state->acc->reg_a == 0xe) ? true : false;
	if(b) { 
		if (get_carry_flag(state) != 1)
			b = false;
	}		
	print_result(b, "DECODER ADD_A Test003");

	return 0;
}

int test_decoder_add_b(struct td4_state *state)
{
	bool b;
	int i;

	state = reset_state(state);

	// Add 0x01 to regster B 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x51;
	
	decoder(state);

	// register B should be 0x0f.
	b = (state->acc->reg_b == 0x0f) ? true : false;
	if(b) {
		// last execution is "add A, 0".
		// so, carry should be 0.
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "DECODER ADD_B Test001");

	state = reset_state(state);

	// Add 0x01 to regster B 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x51;
	// then add 0x06.
	state->memory[ADDRESS_SPACE_SIZE - 1] = 0x56;

	decoder(state);

	// register A should be 0x5.
	b = (state->acc->reg_b == 0x5) ? true : false;
	if(b) { 
		// 0x0f + 0x06 will overflow 4bit range.
		if (get_carry_flag(state) != 1)
			b = false;
	}		
	print_result(b, "DECODER ADD_B Test002");

	state = reset_state(state);

	// Add 0x01 to regster B 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x51;
	// then add 0x0f.
	state->memory[ADDRESS_SPACE_SIZE - 1] = 0x5f;

	decoder(state);

	// register A should be 0x0e.
	b = (state->acc->reg_b == 0x0e) ? true : false;
	if(b) { 
		if (get_carry_flag(state) != 1)
			b = false;
	}		
	print_result(b, "DECODER ADD_B Test003");
		
	return 0;

}

int test_decoder_jmp(struct td4_state *state)
{
	bool b;
	int i;

	state = reset_state(state);

	// Add 0x01 to regster B 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x51;

	// jmp to 0x0f.
	state->memory[10] = 0xff;

	decoder(state);

	// register B should be 0x0a.
	b = (state->acc->reg_b == 0x0a) ? true : false;
	if(b) {
		// last execution is "add A, 0".
		// so, carry should be 0.
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "DECODER JMP Test001");
	return 0;
}

int test_decoder_jnc(struct td4_state *state)
{
	bool b;
	int i;

	state = reset_state(state);

	// Add 0x01 to regster B 15 times.
	for (i = 0; i < ADDRESS_SPACE_SIZE - 1; i++) 
		state->memory[i] = 0x51;

	// mov b, 0

	state->memory[ADDRESS_SPACE_SIZE - 1] = 0x70;

	// jmp to 0x01
	state->memory[6] = 0x71;
	decoder(state);

	// jmp to 0x0f.
	state->memory[7] = 0xff;

	// register B should be 0x0
	b = (state->acc->reg_b == 0x0) ? true : false;
	if(b) {
		// last execution is "mov b, 0".
		// so, carry should be 0.
		if (get_carry_flag(state) != 0)
			b = false;
	}
	print_result(b, "DECODER JNC Test001");
	printf("%d\n", state->acc->reg_b);
	return 0;
}

int test_decoder(void)
{
	struct td4_state *state = init_state();

	test_decoder_add_a(state);
	test_decoder_add_b(state);

	test_decoder_jmp(state);
	test_decoder_jnc(state);

	cleanup_state(state);

	return 0;
	
}

int start_test(void)
{

	int ret;
	
	init_opcode_table();
	
	ret = test_decoder();

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

