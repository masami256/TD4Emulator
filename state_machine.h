#ifndef STATE_MACHONE_H
#define STATE_MACHONE_H 1

// state_machine.c
struct td4_state *init_state(void);
inline void set_carry_flag(struct td4_state *state, u_int8_t flg);

#endif // STATE_MACHONE_H
