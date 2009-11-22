#include "td4emu.h"
#include "state_machine.h"
#include "xmalloc.h"

#include <string.h>

static u_int8_t *init_ip(void)
{
	u_int8_t *ip = xmalloc(sizeof(u_int8_t));

	*ip = 0;

	return ip;
}

inline void set_carry_flag(struct td4_state *state, u_int8_t flg)
{
	state->flags->carry = flg;
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

	state->ip = init_ip();

	memset(state->memory, 0x0, sizeof(state->memory));

	return state;
}



