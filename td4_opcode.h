#ifndef TD4_OPCODE_H
#define TD4_OPCODE_H 1

#include "td4emu.h"

// td4_opcode.c
void init_opcode_table(void);
void cleanup_opcode_table(void);
bool parse_opecode(struct td4_state *state, u_int8_t data);

#endif // TD4_OPCODE_H
