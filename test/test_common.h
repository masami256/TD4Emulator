#ifndef TEST_COMMON_H
#define TEST_COMMON_H 1

#include "../td4emu.h"

void print_result(bool b, char *msg);

int get_total_test_count(void);
int get_total_pass_count(void);
int get_total_fail_count(void);

#endif // TEST_COMMON_H
