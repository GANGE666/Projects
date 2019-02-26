
#ifndef MYLOCK_CONFIG

#define MYLOCK_CONFIG

//是否重定向stdout至日志文件
//#define LOG_MODE

// 主循环TIME_SLICE ms执行一次
#define TIME_SLICE 64
#define SLEEP_TIME_COUNT 100 


//进入更改密码模式的特殊键
#define KEY_ENTRY_CHANGE_PWD 127
#define CHANGEPWD_COUNT 5

//密码长度
#define KEY_LEN 10

//最大密码错误次数
#define MAX_CHECK_PSW_FAIL 5

//盐值
#define SALT "12345678900987654321"

//hash长度
#define HASH_LEN 64

// LED传入参数的宏定义
#define LED_CONTINUE 			0 //继续当前状态
#define LED_SLEEP				2 //睡眠模式
#define LED_PLZ_INPUT_PSW		3 //提示用户输入密码
#define LED_PSW_CORRECT			4 //密码正确
#define LED_PSW_WRONG			5 //密码错误
#define LED_PLZ_INPUT_NEW_PSW	6 //提示用户输入新密码
#define LED_PLZ_REPEAT_NEW_PSW	7 //提示用户再次输入新密码
#define LED_ADD_ONE_CHAR		8 //输入了一个字符
#define LED_CHANING_PWD			9 //提示用户进入修改密码模式请输入密码

//LED每一帧持续时间
#define LED_TIME_OF_CONTINUE 			10	 //继续当前状态
#define LED_TIME_OF_SLEEP				0	 //睡眠模式
#define LED_TIME_OF_PLZ_INPUT_PSW		10	 //提示用户进入修改密码模式请输入密码
#define LED_TIME_OF_PSW_CORRECT			10	 //密码正确
#define LED_TIME_OF_PSW_WRONG			10	 //密码错误
#define LED_TIME_OF_PLZ_INPUT_NEW_PSW	10	 //提示用户输入新密码
#define LED_TIME_OF_PLZ_REPEAT_NEW_PSW	10	 //提示用户再次输入新密码

//密码文件路径及个数
#define KEY_FILE_NUM 3

//文件信息
#define MAX_CMP_FILE 10
#define MAX_FILE_SIZE 1000

//是否使用密钥文件压码
#define KEYFILE_CHECK_CODE

//看门狗缓冲区目录
#define WCD_BUF_PATH "wcd_buf/buf"

//读写文件标志
#define FILE_FLAG_PATH "mylock/fileflag"

#endif









