#ifndef TD4EMU_h
#define TD4EMU_H 1

#include <sys/types.h>
#include <assert.h>

// TD4 has two registers which A register and B register.
// Register size is 4 bit. 
struct td4_registers {
	u_int8_t reg_a;
	u_int8_t reg_b;
};

// TD4 has only one flag register.
struct td4_flag_registers {
	u_int8_t carry;
};

// Memory space is 16 bytes
#define ADDRESS_SPACE_SIZE 16

// td4emu.c
int start_emulation(char *file);

// td4_opcode.c
void init_opcode_table(void);
void cleanup_opcode_table(void);
u_int8_t parse_opecode(u_int8_t data);

#endif // TD4EMU_H
