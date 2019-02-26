CROSS = armv4l-unknown-linux-
CC= $(CROSS)gcc
EXP_INSTALL = install -m 755 

EXTRA_LIBS += -lpthread

EXEC= mylock
OBJS= ./global_var.o file_check/file_flag.o file_check/file_cmp.o file_encry/keyfilemanager.o state_check/state_check.o keyboard/keyboard_main.o keyboard/keyboard.o keyboard/get_key.o led/led.o md5/do_md5.o motor/motor_up.o ./random_clac.o ./cmp_hash.o ./mylock_init.o ./led_thread.o ./main_misc.o ./main_v2.0.o

all: $(EXEC)

# 剥离符号表则使用：
#	$(CC) -s -o $@ $(OBJS)  $(EXTRA_LIBS)

$(EXEC): $(OBJS)
	$(CC)  -o $@ $(OBJS)  $(EXTRA_LIBS)

install:
	$(EXP_INSTALL) $(EXEC) $(INSTALL_DIR)


clean:
	-rm -f $(EXEC) *.elf *.gdb *.o file_check/*.o file_encry/*.o keyboard/*.o led/*.o md5/*.o motor/*.o state_check/*.o
