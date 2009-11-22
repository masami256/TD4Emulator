CC = gcc

CFLAGS = -g -Wall -O2

test_add = test_add
test_mov = test_mov

objs = ../td4emu.o \
	../td4_opcode.o \
	../xmalloc.o \
	../state_machine.o \
	../td4_abort.o \
	test_common.o

all: $(test_add) $(test_mov)

$(test_add): $(objs) $(test_add).o
	$(CC) $(objs) $(test_add).o -o $(test_add)

$(test_mov): $(objs) $(test_mov).o
	$(CC) $(objs) $(test_mov).o -o $(test_mov)

test:
	./$(test_add)
	./$(test_mov)
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(test_add) $(test_mov) core *.o *~

.PHONY: $(test_add) $(test_mov)