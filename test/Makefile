CC = gcc

CFLAGS = -g -Wall

test_add = test_add
test_mov = test_mov
test_jmp = test_jmp
test_jnc = test_jnc
test_in  = test_in

objs = ../td4emu.o \
	../td4_opcode.o \
	../xmalloc.o \
	../state_machine.o \
	../td4_abort.o \
	test_common.o

all: $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in)

$(test_add): $(objs) $(test_add).o
	$(CC) $(objs) $(test_add).o -o $(test_add)

$(test_mov): $(objs) $(test_mov).o
	$(CC) $(objs) $(test_mov).o -o $(test_mov)

$(test_jmp): $(objs) $(test_jmp).o
	$(CC) $(objs) $(test_jmp).o -o $(test_jmp)

$(test_jnc): $(objs) $(test_jnc).o
	$(CC) $(objs) $(test_jnc).o -o $(test_jnc)

$(test_in): $(objs) $(test_in).o
	$(CC) $(objs) $(test_in).o -o $(test_in)

test:
	./$(test_add)
	./$(test_mov)
	./$(test_jmp)
	./$(test_jnc)
	./$(test_in)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in) core *.o *~

.PHONY: $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in)
