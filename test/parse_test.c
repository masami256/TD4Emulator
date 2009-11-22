#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../td4emu.h"

int test_parse_opcode(void)
{
	// 0x03: 0000 0011
	printf("test: op is 0000 | im is 0011\n");
	parse_opecode(0x03);

	// 0x76: 0111 0110 
	printf("test: op is 0111 | im is 0110\n");
	parse_opecode(0x76);

	return 0;
	
}

int start_test(void)
{
	struct td4_registers regs;
	int ret;

	memset(&regs, 0, sizeof(regs));

	init_opcode_table();
	
	ret = test_parse_opcode();

	cleanup_opcode_table();

	return ret;
}

int main(int argc, char **argv)
{
	return start_test();
}

