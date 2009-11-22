CC = gcc

CFLAGS = -g -Wall -O2

parse_test = parse_test

objs = ../td4emu.o \
	../td4_opcode.o \
	../xmalloc.o \
	../state_machine.o

$(parse_test): $(objs) $(parse_test).o
	$(CC) $(objs) $(parse_test).o -o $(parse_test)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(parse_test) core *.o *~
