CC = gcc

CFLAGS = -g -Wall -O2

target = td4emu

objs = td4emu.o \
	td4_opcode.o \
	xmalloc.o

$(target): $(objs)
	$(CC) $(objs) -o $(target)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(target) core *.o *~
