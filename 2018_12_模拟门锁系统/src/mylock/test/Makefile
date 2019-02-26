CROSS = armv4l-unknown-linux-
CC= gcc
EXP_INSTALL = install -m 755 

EXTRA_LIBS += -lpthread

EXEC= test
OBJS= ../file_check/file_cmp.o ../file_encry/keyfilemanager.o ../keyboard/keyboard_main.o ../keyboard/keyboard.o ../keyboard/get_key.o ../led/led.o ../md5/do_md5.o ../motor/motor_up.o ../cmp_hash.o ../mylock_init.o ../main_misc.o ./test.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC)  -o $@ $(OBJS)  $(EXTRA_LIBS)

install:
	$(EXP_INSTALL) $(EXEC) $(INSTALL_DIR)


clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
