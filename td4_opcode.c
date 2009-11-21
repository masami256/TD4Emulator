#include "td4emu.h"
#include "xmalloc.h"

#include <stdio.h>

u_int8_t test0(u_int8_t im)
{
	printf("test0\n");
	return 0;
}
u_int8_t test1(u_int8_t im)
{
	printf("test1\n");
	return 0;
}
u_int8_t test2(u_int8_t im)
{
	printf("test2\n");
	return 0;
}
u_int8_t test3(u_int8_t im)
{
	printf("test3\n");
	return 0;
}
u_int8_t test4(u_int8_t im)
{
	printf("test4\n");
	return 0;
}
u_int8_t test5(u_int8_t im)
{
	printf("test5\n");
	return 0;
}
u_int8_t test6(u_int8_t im)
{
	printf("test6\n");
	return 0;
}
u_int8_t test7(u_int8_t im)
{
	printf("test7\n");
	return 0;
}
u_int8_t test8(u_int8_t im)
{
	printf("test8\n");
	return 0;
}
u_int8_t test9(u_int8_t im)
{
	printf("test9\n");
	return 0;
}
u_int8_t test10(u_int8_t im)
{
	printf("test10\n");
	return 0;
}
u_int8_t test11(u_int8_t im)
{
	printf("test11\n");
	return 0;
}
u_int8_t test12(u_int8_t im)
{
	printf("test12\n");
	return 0;
}
u_int8_t test13(u_int8_t im)
{
	printf("test14\n");
	return 0;
}
u_int8_t test14(u_int8_t im)
{
	printf("test14\n");
	return 0;
}
u_int8_t test15(u_int8_t im)
{
	printf("test15\n");
	return 0;
}


// OPCODE for TD4.
struct opcode {
	u_int8_t op;
	u_int8_t (*func)(u_int8_t im);
};

// OPCODE for TD4.
static struct opcode opcodes[] = {
	// ADD functions
	{ 0x00, &test0 }, // 0000: ADD A, Im
	{ 0x05, &test5 }, // 0101: AAD B, Im
	
	// MOV functions
	// Moving imediation data to A or B register.
	{ 0x03, &test3 }, // 0011: MOV A, Im
	{ 0x07, &test7 }, // 0111: MOV B, Im

	// Mov data from register to register.
	{ 0x01, &test1 }, // 0001: MOV A, B
	{ 0x04, &test4 }, // 0100: MOV B, A

	// JMP function.
	{ 0x0f, &test15 }, // 1111: JMP Im

	// JMP if a condition is true.
	{ 0x0e, &test14 }, // 1110: JNC Im
	
	// IN functions.
	{ 0x10, &test10 }, // 0010: IN A
	{ 0x06, &test6 }, // 0110: IN B

	// OUT functions.
	{ 0x09, &test9 }, // 1001: OUT B
	{ 0x0b, &test11 }, // 1011: OUT Im

	// NOP
	// it same as  ADD A, 0
};

#define OPCODE_COUNT (sizeof(opcodes) / sizeof(opcodes[0]))

struct opcode_table {
	struct opcode *op;
};

static struct opcode_table *op_table;

void init_opcode_table(void)
{
	int i;

	op_table = xmalloc(sizeof(struct opcode_table *) * OPCODE_COUNT);
	assert(op_table != NULL);

	for (i = 0; i < OPCODE_COUNT; i++)
		op_table[i].op = NULL;

	for (i = 0; i < OPCODE_COUNT; i++) 
		op_table[opcodes[i].op].op = &opcodes[i];

	for (i = 0; i < OPCODE_COUNT; i++) {
		if (op_table[i].op != NULL)
			op_table[i].op->func(0);
	}

}

void cleanup_opcode_table(void)
{
	xfree(op_table);
}

