
#ifndef MYLOCK_CONFIG

#define MYLOCK_CONFIG

//�Ƿ��ض���stdout����־�ļ�
//#define LOG_MODE

// ��ѭ��TIME_SLICE msִ��һ��
#define TIME_SLICE 64
#define SLEEP_TIME_COUNT 100 


//�����������ģʽ�������
#define KEY_ENTRY_CHANGE_PWD 127
#define CHANGEPWD_COUNT 5

//���볤��
#define KEY_LEN 10

//�������������
#define MAX_CHECK_PSW_FAIL 5

//��ֵ
#define SALT "12345678900987654321"

//hash����
#define HASH_LEN 64

// LED��������ĺ궨��
#define LED_CONTINUE 			0 //������ǰ״̬
#define LED_SLEEP				2 //˯��ģʽ
#define LED_PLZ_INPUT_PSW		3 //��ʾ�û���������
#define LED_PSW_CORRECT			4 //������ȷ
#define LED_PSW_WRONG			5 //�������
#define LED_PLZ_INPUT_NEW_PSW	6 //��ʾ�û�����������
#define LED_PLZ_REPEAT_NEW_PSW	7 //��ʾ�û��ٴ�����������
#define LED_ADD_ONE_CHAR		8 //������һ���ַ�
#define LED_CHANING_PWD			9 //��ʾ�û������޸�����ģʽ����������

//LEDÿһ֡����ʱ��
#define LED_TIME_OF_CONTINUE 			10	 //������ǰ״̬
#define LED_TIME_OF_SLEEP				0	 //˯��ģʽ
#define LED_TIME_OF_PLZ_INPUT_PSW		10	 //��ʾ�û������޸�����ģʽ����������
#define LED_TIME_OF_PSW_CORRECT			10	 //������ȷ
#define LED_TIME_OF_PSW_WRONG			10	 //�������
#define LED_TIME_OF_PLZ_INPUT_NEW_PSW	10	 //��ʾ�û�����������
#define LED_TIME_OF_PLZ_REPEAT_NEW_PSW	10	 //��ʾ�û��ٴ�����������

//�����ļ�·��������
#define KEY_FILE_NUM 3

//�ļ���Ϣ
#define MAX_CMP_FILE 10
#define MAX_FILE_SIZE 1000

//�Ƿ�ʹ����Կ�ļ�ѹ��
#define KEYFILE_CHECK_CODE

//���Ź�������Ŀ¼
#define WCD_BUF_PATH "wcd_buf/buf"

//��д�ļ���־
#define FILE_FLAG_PATH "mylock/fileflag"

#endif









