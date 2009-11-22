#include "td4emu.h"
#include "xmalloc.h"

#include <stdio.h>

// OPCODE for TD4.
struct opcode {
	u_int8_t op;
	u_int8_t (*func)(u_int8_t im);
};

// OPCODE for TD4.
static struct opcode opcodes[] = {
	// ADD functions
	{ 0x00, NULL }, // 0000: ADD A, Im
	{ 0x05, NULL }, // 0101: AAD B, Im
	
	// MOV functions
	// Moving imediation data to A or B register.
	{ 0x03, NULL }, // 0011: MOV A, Im
	{ 0x07, NULL }, // 0111: MOV B, Im

	// Mov data from register to register.
	{ 0x01, NULL }, // 0001: MOV A, B
	{ 0x04, NULL }, // 0100: MOV B, A

	// JMP function.
	{ 0x0f, NULL }, // 1111: JMP Im

	// JMP if a condition is true.
	{ 0x0e, NULL }, // 1110: JNC Im
	
	// IN functions.
	{ 0x10, NULL }, // 0010: IN A
	{ 0x06, NULL }, // 0110: IN B

	// OUT functions.
	{ 0x09, NULL }, // 1001: OUT B
	{ 0x0b, NULL }, // 1011: OUT Im

	// NOP
	// it same as  ADD A, 0
};

#define OPCODE_COUNT (sizeof(opcodes) / sizeof(opcodes[0]))

struct opcode_table {
	struct opcode *op;
};

static struct opcode_table *op_table;
static struct td4_flag_registers td4_flags;

// PRIVATE FUNCTIONS
static u_int8_t add(u_int8_t reg, u_int8_t im)
{
	u_int8_t ret = 0;

	// clear carry flag before add.
	td4_flags.carry = 0;

	ret = reg + im;
	if (ret > 0x0f)
		td4_flags.carry = 1;

	return ret;
}

static u_int8_t add_a(struct td4_registers *registers, u_int8_t im)
{
	return add(registers->reg_a, im);
}

static u_int8_t add_b(struct td4_registers *registers, u_int8_t im)
{
	return add(registers->reg_b, im);
}

static void dump_operand(u_int8_t op, u_int8_t im)
{
	char op_c[4] = { 0x00 };
	char op_i[4] = { 0x00 };
	u_int8_t op_tmp, im_tmp;
	int i;

	op_tmp = op;
	im_tmp = im;

	for (i = 4; i > 0; i--, op_tmp >>= 1)
		op_c[i - 1] =  (op_tmp & 0x01) ? '1' : '0';
	
	for (i = 4; i > 0; i--, im_tmp >>= 1)
		op_i[i - 1] =  (im_tmp & 0x01) ? '1' : '0';

	printf("op is %s:%02x | im is %s:%02x\n", op_c, op, op_i, im);
}

// PUBLIC FUNTIONS
void init_opcode_table(void)
{
	int i;

	op_table = xmalloc(sizeof(struct opcode_table *) * OPCODE_COUNT);
	assert(op_table != NULL);

	for (i = 0; i < OPCODE_COUNT; i++)
		op_table[i].op = NULL;

	for (i = 0; i < OPCODE_COUNT; i++) 
		op_table[opcodes[i].op].op = &opcodes[i];
}

void cleanup_opcode_table(void)
{
	xfree(op_table);
}

u_int8_t parse_opecode(u_int8_t data)
{
	u_int8_t op, im, ret;

	ret = -1;
	op = data >> 4;
	im = data & 0x0f;
	
	dump_operand(op, im);
/*
	if (op >= 0x00 && op <= 0x0f)
		ret = op_table[op].op->func(im);
*/
	return ret;
}
