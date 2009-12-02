#ifndef STATE_MACHONE_H
#define STATE_MACHONE_H 1

// state_machine.c
struct td4_state *init_state(void);
void cleanup_state(struct td4_state *state);
struct td4_state *reset_state(struct td4_state *state);
void *decoder(struct td4_state *state);

inline void set_carry_flag(struct td4_state *state, u_int8_t flg);
inline u_int8_t get_carry_flag(struct td4_state *state);
inline void set_ip(struct td4_state *state, u_int8_t val);
inline u_int8_t get_ip(struct td4_state *state);
inline void inrement_ip(struct td4_state *state);
inline u_int8_t fetch(struct td4_state *state);

#endif // STATE_MACHONE_H
