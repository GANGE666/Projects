

// 每隔LOCK_GUARD_TIME_SLICE ms 运行一次循环
#define LOCK_GUARD_TIME_SLICE 64 << 2

// 连续WCD_RESTART_TIME 次无变化重启程序
#define WCD_RESTART_TIME 10

// 连续MAX_TIME_RESTART_FAIL 次重启程序失败将重启机器
#define MAX_TIME_RESTART_FAIL 5

// 看门狗缓冲区大小
#define WCD_BUF_SIZE 64

// 缓冲区文件名
char wcd_buf_fname[] = "wcd_buf/buf";


// mylock文件及备份文件位置
char * MYLOCK_AND_BAK_PATH[] = {
	"mylock/mylock",
	"bak1/mylock",
	"bak2/mylock"
};
#define MYLOCK_FILE_CHECK_NUM 3

// 记录keyfile修改次数的文件
char KEYFILE_COUNTER_PATH[] = "mylock/keyfile/counter";
// 达到一定次数就需要更换，用字符串表示，直接比较即可 以下为2**16
unsigned char MAX_CHANGE_KEYFILE[] = {0xFF,0xFF};
#define KEYFILE_BUFF_SIZE 5


//keyfile位置
char * KEY_FILE_PATH[] = {"mylock/keyfile/keyfile1", "mylock/keyfile/keyfile2", "mylock/keyfile/keyfile3"};
#define KEYFILE_NUM 3

#define MAX_FILE_SIZE 1000

#define FILE_PATH_SIZE 100
