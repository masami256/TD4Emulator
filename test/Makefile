CC = gcc

CFLAGS = -g -Wall

test_add = test_add
test_mov = test_mov
test_jmp = test_jmp
test_jnc = test_jnc
test_in  = test_in
test_out = test_out
test_decoder = test_decoder
test_bin2dec = test_bin2dec

objs = ../td4emu.o \
	../td4_decoder.o \
	../xmalloc.o \
	../state_machine.o \
	../td4_abort.o \
	../td4_utils.o \
	test_common.o

all: $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in) $(test_out) $(test_decoder) $(test_bin2dec)

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

$(test_out): $(objs) $(test_out).o
	$(CC) $(objs) $(test_out).o -o $(test_out)

$(test_decoder): $(objs) $(test_decoder).o
	$(CC) $(objs) $(test_decoder).o -o $(test_decoder)

$(test_bin2dec): $(objs) $(test_bin2dec).o
	$(CC) $(objs) $(test_bin2dec).o -o $(test_bin2dec)

test:
	./$(test_add)
	./$(test_mov)
	./$(test_jmp)
	./$(test_jnc)
	./$(test_in)
	./$(test_out)
	./$(test_decoder)
	./$(test_bin2dec)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in) $(test_out) $(test_decoder) $(test_bin2dec) core *.o *~

.PHONY: $(test_add) $(test_mov) $(test_jmp) $(test_jnc) $(test_in) $(test_out) $(test_decoder) $(test_bin2dec)

