CC = gcc

CFLAGS = -g -Wall -O2

target = td4emu

objs = main.o \
	td4emu.o \
	state_machine.o \
	td4_opcode.o \
	xmalloc.o


test_targets = test/parse_test.o

$(target): $(objs)
	$(CC) $(objs) -o $(target)

test: $(test_targets)
	cd ./test && make
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(target) core *.o *~
	cd ./test && make clean