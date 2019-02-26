CROSS = armv4l-unknown-linux-
CC= $(CROSS)gcc
EXP_INSTALL = install -m 755 

EXTRA_LIBS += -lpthread

EXEC= lock_guard
OBJS= file_check/file_cmp.o main.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC)  -o $@ $(OBJS)  $(EXTRA_LIBS)

install:
	$(EXP_INSTALL) $(EXEC) $(INSTALL_DIR)


clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
