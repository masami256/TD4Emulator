CC = gcc

CFLAGS = -g -Wall 
CFLAGS += `pkg-config --cflags gtk+-2.0`

#LD_FLAGS = `pkg-config  --libs glib-2.0`
LD_FLAGS = `pkg-config  --libs gtk+-2.0`

target = td4emu

objs = main.o \
	td4emu.o \
	state_machine.o \
	td4_decoder.o \
	xmalloc.o \
	td4_utils.o \
	td4_abort.o

$(target): $(objs)
	$(CC) $(LD_FLAGS) $(objs) -o $(target)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(target) core *.o *~
