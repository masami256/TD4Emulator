#include "td4emu.h"
#include "td4_utils.h"

u_int8_t bin2dec(char *bits, int size)
{
	u_int8_t ret;
	int i;

	i = ret = 0;
	
	for (i = 0; i < size; i++) 
		ret |= bits[i] << i;
 
	return ret;
}

