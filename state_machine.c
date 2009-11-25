#include "td4emu.h"
#include "state_machine.h"
#include "td4_decoder.h"
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

static inline u_int8_t fetch(struct td4_state *state)
{
	return state->memory[state->ip];
}

static inline u_int8_t get_ip(struct td4_state *state)
{
	return state->ip;
}

inline void set_ip(struct td4_state *state, u_int8_t val)
{
	state->ip = val;
}

inline void inrement_ip(struct td4_state *state)
{
	state->ip++;
}

void *decoder(struct td4_state *state)
{
	while (get_ip(state) < ADDRESS_SPACE_SIZE) 
		parse_opecode(state, fetch(state));

	return NULL;
}

struct td4_state *init_state(void)
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

	state->io = xmalloc(sizeof(struct td4_io_port));
	state->io->in_port = state->io->out_port = 0;

	return state;
}

void cleanup_state(struct td4_state *state)
{
	xfree(state->acc);
	xfree(state->flags);
	state->ip = 0;
	xfree(state->io);
	xfree(state);
}

struct td4_state *
reset_state(struct td4_state *state)
{
	state->acc->reg_a = state->acc->reg_b = 0;
	state->flags->carry = 0;
	memset(state->memory, 0x0, sizeof(state->memory));

	state->ip = 0;

	state->io->in_port = state->io->out_port = 0;

	return state;
}
