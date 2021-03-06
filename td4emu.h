#ifndef TD4EMU_H 
#define TD4EMU_H 1

#include <sys/types.h>

// TD4 has two registers which A register and B register.
// Register size is 4 bit. 
struct td4_acc_registers {
	u_int8_t reg_a;
	u_int8_t reg_b;
};

// TD4 has only one flag register.
struct td4_flag_registers {
	u_int8_t carry;
};

// Memory space is 16 bytes
#define ADDRESS_SPACE_SIZE 16

struct td4_io_port {
	u_int8_t in_port;
	u_int8_t out_port;
};

struct td4_state {
	struct td4_acc_registers *acc;
	struct td4_flag_registers *flags;
	u_int8_t ip;
	u_int8_t memory[ADDRESS_SPACE_SIZE];
	struct td4_io_port *io;
};

typedef enum {
	false = 0,
	true,
} bool;

#define IO_PORT_BIT_SIZE 4

// td4emu.c
int start_emulation(char *file);

// td4_abort.c
void td4_abort(char *msg);

#endif // TD4EMU_H
