#ifndef TD4_OPCODE_H
#define TD4_OPCODE_H 1

// td4_opcode.c
void init_opcode_table(void);
void cleanup_opcode_table(void);
u_int8_t parse_opecode(u_int8_t data);

#endif // TD4_OPCODE_H
