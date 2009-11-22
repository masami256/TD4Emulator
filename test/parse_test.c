#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../td4emu.h"
#include "../td4_opcode.h"

int test_parse_opcode(void)
{
	// 0x03: 0000 0011
	printf("\x1b[1;32mtest: op is 0000 | im is 0011\n");
	printf("\x1b[0m");

	parse_opecode(0x03);

	// 0x76: 0111 0110 
	printf("\x1b[1;32mtest: op is 0111 | im is 0110\n");
	printf("\x1b[0m");
	parse_opecode(0x76);

	return 0;
	
}

int start_test(void)
{

	int ret;


	init_opcode_table();
	
	ret = test_parse_opcode();

	cleanup_opcode_table();

	return ret;
}

int main(int argc, char **argv)
{
	return start_test();
}

