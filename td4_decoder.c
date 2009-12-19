#include "td4emu.h"
#include "xmalloc.h"
#include "state_machine.h"

#include <stdio.h>

// ADD functions.
static u_int8_t add(struct td4_state *state, u_int8_t reg, u_int8_t im);
static u_int8_t add_a(struct td4_state *state, u_int8_t im);
static u_int8_t add_b(struct td4_state *state, u_int8_t im);

// MOV functions.
static u_int8_t mov(struct td4_state *state, u_int8_t reg, u_int8_t im);
static u_int8_t mov_a(struct td4_state *state, u_int8_t im);
static u_int8_t mov_b(struct td4_state *state, u_int8_t im);
static u_int8_t mov_a2b(struct td4_state *state, u_int8_t im);
static u_int8_t mov_b2a(struct td4_state *state, u_int8_t im);

// JMP function
static u_int8_t jmp(struct td4_state *state, u_int8_t im);

// JNC function
static u_int8_t jnc(struct td4_state *state, u_int8_t im);

// IN functions
static u_int8_t in_a(struct td4_state *state, u_int8_t im);
static u_int8_t in_b(struct td4_state *state, u_int8_t im);

// OUT functions
static u_int8_t out_im(struct td4_state *state, u_int8_t im);
static u_int8_t out_b(struct td4_state *state, u_int8_t im);

// OPCODE for TD4.
struct opcode {
	u_int8_t op;
	u_int8_t (*func)(struct td4_state *state, u_int8_t im);
};


// OPCODE for TD4.
static struct opcode opcodes[] = {
	// ADD functions
	{ 0x00, add_a }, // 0000: ADD A, Im
	{ 0x05, add_b }, // 0101: AAD B, Im
	
	// MOV functions
	// Moving imediation data to A or B register.
	{ 0x03, mov_a }, // 0011: MOV A, Im
	{ 0x07, mov_b }, // 0111: MOV B, Im

	// Mov data from register to register.
	{ 0x01, mov_b2a }, // 0001: MOV A, B
	{ 0x04, mov_a2b }, // 0100: MOV B, A

	// JMP function.
	{ 0x0f, jmp }, // 1111: JMP Im

	// JMP if a condition is true.
	{ 0x0e, jnc }, // 1110: JNC Im
	
	// IN functions.
	{ 0x02, in_a }, // 0010: IN A
	{ 0x06, in_b }, // 0110: IN B

	// OUT functions.
	{ 0x09, out_b }, // 1001: OUT B
	{ 0x0b, out_im }, // 1011: OUT Im

	// NOP
	// it same as  ADD A, 0
};

#define OPCODE_MAX_VALUE (0x0f + 0x01)

struct opcode_table {
	struct opcode *op;
};

static struct opcode_table *op_table;

// PRIVATE FUNCTIONS

static u_int8_t add(struct td4_state *state, u_int8_t reg, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);

	u_int8_t ret = 0;

	ret = reg + im;
	if (ret > 0x0f) {
		ret &= 0x0f;
		set_carry_flag(state, 1);
	}

	return ret;
}

static u_int8_t add_a(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_a = add(state, state->acc->reg_a, im);
	return 1;
}

static u_int8_t add_b(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_b = add(state, state->acc->reg_b, im);
	return 1;
}

static u_int8_t mov(struct td4_state *state, u_int8_t reg, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);

	reg = im;
	return reg;
}

static u_int8_t mov_a(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_a =  mov(state, state->acc->reg_a, im);
	return 1;
}

static u_int8_t mov_b(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_b = mov(state, state->acc->reg_b, im);
	return 1;
}

static u_int8_t mov_a2b(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_b = mov(state, state->acc->reg_b, state->acc->reg_a);
	return 1;
}

static u_int8_t mov_b2a(struct td4_state *state, u_int8_t im)
{
	state->acc->reg_a = mov(state, state->acc->reg_a, state->acc->reg_b);
	return 1;
}

static u_int8_t jmp(struct td4_state *state, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);

	set_ip(state, im);
	return 0;
}

static u_int8_t jnc(struct td4_state *state, u_int8_t im)
{
	if (!get_carry_flag(state)) 
		set_ip(state, im);

	// clear carry flag after execute opecode.
	set_carry_flag(state, 0);

	return 0;
}


static u_int8_t in_a(struct td4_state *state, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);
	state->acc->reg_a = state->io->in_port & 0x0f;
	return 1;
}

static u_int8_t in_b(struct td4_state *state, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);
	state->acc->reg_b = state->io->in_port & 0x0f;
	return 1;
}

static u_int8_t out_im(struct td4_state *state, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);
	state->io->out_port = im;
	return 1;
}

static u_int8_t out_b(struct td4_state *state, u_int8_t im)
{
	// clear carry flag before execute opecode.
	set_carry_flag(state, 0);
	state->io->out_port = state->acc->reg_b;
	return 1;
}

// for debuging
#ifdef TD4_DEBUG
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

	printf("op is %s:0x%02x | im is %s:0x%02x\n", op_c, op, op_i, im);
}
#else
static void dump_operand(u_int8_t op, u_int8_t im) {}
#endif // TD4_DEBUG

// PUBLIC FUNTIONS
void init_opcode_table(void)
{
	int i;

	op_table = xmalloc(sizeof(struct opcode_table *) * OPCODE_MAX_VALUE);

	for (i = 0; i < OPCODE_MAX_VALUE; i++)
		op_table[i].op = NULL;

	for (i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++)
	     op_table[opcodes[i].op].op = &opcodes[i];
}

void cleanup_opcode_table(void)
{
	xfree(op_table);
}

bool parse_opecode(struct td4_state *state, u_int8_t data)
{
	u_int8_t op, im, ret;

	ret = false;
	op = data >> 4;
	im = data & 0x0f;
	
	dump_operand(op, im);

	// op should already be between 0 to 0xf.
	// it doesn't need to check its range.
	if (op_table[op].op)
		ret = op_table[op].op->func(state, im);

	return ret;
}
