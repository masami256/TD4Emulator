#include "test_common.h"
#include "../td4_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_bin2dec_8(void)
{
	char data[8];
	int ret;
	bool b;

	memset(data, 0x0, sizeof(data));

	// 0000 0010
	memset(data, 0x0, sizeof(data));
	data[1] = 1;
	ret = bin2dec(data, 8);
	b = ret == 0x2 ? true : false;
	print_result(b, "bin2dec_8 Test001");		
	
	// 1000 0000
	memset(data, 0x0, sizeof(data));
	data[7] = 1;
	ret = bin2dec(data, 8);
	b = ret == 0x80 ? true : false;
	print_result(b, "bin2dec_8 Test002");		
	
	// 1010 1010
	memset(data, 0x0, sizeof(data));
	data[1] = data[3] = data[5] = data[7] = 1;
	ret = bin2dec(data, 8);
	b = ret == 0xaa ? true : false;
	print_result(b, "bin2dec_8 Test003");		

	// 0101 0101
	memset(data, 0x0, sizeof(data));
	data[0] = data[2] = data[4] = data[6] = 1;
	ret = bin2dec(data, 8);
	b = ret == 0x55 ? true : false;
	print_result(b, "bin2dec_8 Test004");		

	// 0000 0000
	memset(data, 0x0, sizeof(data));
	ret = bin2dec(data, 8);
	b = ret == 0x0 ? true : false;
	print_result(b, "bin2dec_8 Test005");		

	return 0;
	
}

int test_bin2dec_4(void)
{
	char data[8];
	int ret;
	bool b;

	memset(data, 0x0, sizeof(data));

	// 1010
	memset(data, 0x0, sizeof(data));
	data[0] = 0;
	data[1] = 1;
	data[2] = 0;
	data[3] = 1;
	ret = bin2dec(data, 4);
	b = ret == 0xa ? true : false;
	print_result(b, "bin2dec_4 Test001");		
	
	// 0101
	memset(data, 0x0, sizeof(data));
	data[0] = 1;
	data[1] = 0;
	data[2] = 1;
	ret = bin2dec(data, 4);
	b = ret == 0x5 ? true : false;
	print_result(b, "bin2dec_4 Test002");		
	
	// 1111
	memset(data, 0x0, sizeof(data));
	data[0] = data[1] = data[2] = data[3] = 1;
	ret = bin2dec(data, 4);
	b = ret == 0xf ? true : false;
	print_result(b, "bin2dec_4 Test003");		

	// 0000
	memset(data, 0x0, sizeof(data));
	ret = bin2dec(data, 4);
	b = ret == 0x0 ? true : false;
	print_result(b, "bin2dec_4 Test004");		

	return 0;
	
}


int start_test(void)
{

	int ret;
	
	ret = test_bin2dec_8();
	ret = test_bin2dec_4();
	printf("Total test case is %d\n", get_total_test_count());
	printf("Total passed test case is %d\n", get_total_pass_count());
	printf("Total failed test case is %d\n", get_total_fail_count());

	return ret;
}

int main(int argc, char **argv)
{
	return start_test();
}

