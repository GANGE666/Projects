/*
state_check.h

定义了各个状态的二进制位以及其之间的关系

*/

#define STATE_CHECK_OPEN



// 启动电机处理链
#define STATE_ENTER_ED					1<<2
#define STATE_HASH_ED					1<<3
#define STATE_CHECK_PSW_ED				1<<4
#define STATE_MOTOR_UP					1<<5

#define REQ_STATE_ENTER_ED					0
#define REQ_STATE_HASH_ED					1<<2
#define REQ_STATE_CHECK_PSW_ED				1<<3
#define REQ_STATE_MOTOR_UP					1<<4


// 更换密码处理链
#define STATE_IN_CHANGE_PWD_MODE		1<<1
#define STATE_SAVE_PSW_1				1<<6
#define STATE_SAVE_PSW_2				1<<7
#define STATE_UPDATE_KEYFILE			1<<8

#define REQ_STATE_IN_CHANGE_PWD_MODE		0
#define REQ_STATE_SAVE_PSW_1				1<<4
#define REQ_STATE_SAVE_PSW_2				1<<6
#define REQ_STATE_UPDATE_KEYFILE			1<<7





