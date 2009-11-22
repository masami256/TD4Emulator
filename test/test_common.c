#include "test_common.h"
#include <stdio.h>

static int test_count;
static int pass_count;
static int fail_count;

void print_result(bool b, char *msg)
{
	char *color = (b) ? "\x1b[1;32m" : "\x1b[1;31m";
	char *ret = (b) ? "OK" : "NG";

	test_count++;
	if (b)
		pass_count++;
	else
		fail_count++;

	printf("%s%s %s\n", color, msg, ret);

	printf("\x1b[0m");
}

int get_total_test_count(void)
{
	return test_count;
}

int get_total_pass_count(void)
{
	return pass_count;
}

int get_total_fail_count(void)
{
	return fail_count;
}
