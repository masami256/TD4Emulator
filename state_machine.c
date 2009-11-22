#include "td4emu.h"
#include "state_machine.h"
#include "xmalloc.h"

#include <string.h>

inline void set_carry_flag(struct td4_state *state, u_int8_t flg)
{
	state->flags->carry = flg;
}

inline u_int8_t get_carry_flag(struct td4_state *state)
{
	return state->flags->carry;
}

struct td4_state *
init_state(void)
{
	struct td4_state *state;
	
	state = xmalloc(sizeof(struct td4_state));

	state->acc = xmalloc(sizeof(struct td4_acc_registers));
	state->acc->reg_a = 0;
	state->acc->reg_b = 0;

	state->flags = xmalloc(sizeof(struct td4_flag_registers));
	state->flags->carry = 0;

	state->ip = 0;
	memset(state->memory, 0x0, sizeof(state->memory));

	return state;
}

void cleanup_state(struct td4_state *state)
{
	xfree(state->acc);
	xfree(state->flags);
	state->ip = 0;
	xfree(state);
}

struct td4_state *
reset_state(struct td4_state *state)
{
	state->acc->reg_a = state->acc->reg_b = 0;
	state->flags->carry = 0;
	memset(state->memory, 0x0, sizeof(state->memory));

	state->ip = 0;

	return state;
}
