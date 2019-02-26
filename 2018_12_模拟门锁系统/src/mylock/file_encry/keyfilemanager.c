#include<string.h>
#include<stdio.h>
#include<fcntl.h>

#define READ_SIZE 16+1

#define XOR_KEY 0xAA


//�ӽ��ܴ��ļ��ж�ȡ������ֵ 
void decryKeyFileStr(unsigned char * keyfiledata, unsigned char * key){
	int i = 0;
	for(; i < READ_SIZE; i++)
		key[i] = keyfiledata[i] ^ XOR_KEY;
		
	return;	
}

//��ȡ�ļ������� 
int readKeyFile(char * f_path, unsigned char * key){
	int i = 0;
	
	//��ȡ��δ���ܵ�key_data
	unsigned char keyfiledata[READ_SIZE];
	//��ȡ�ļ��Ƿ�ɹ��ı�־���� 
	int readfileflag = 0;
	
	//���ļ���������� 
	int fd;

	printf("enter readKeyFile!\n");


	fd = open(f_path, O_RDONLY);
	if(fd < 0)
	{
		printf("keyfilemanager.c\treadKeyFile()\t*************************KEY FILE OPEN FAIL!*****************************\n");
		return -1;
	}
	
	//��ȡ�ļ���������Ƿ���� 
	readfileflag = read(fd, keyfiledata, READ_SIZE);
	if(readfileflag <= 0){
		printf("keyfilemanager.c\treadKeyFile()\t*************************KEY FILE READ ERROR!*****************************\n");
		return -1;
	}
	close(fd);
	
#ifdef DEBUG
	printf("keyfilemanager.c\treadKeyFile()\t")	;
	for(i=0;i<READ_SIZE;i++)
	{
		printf("%02x",keyfiledata[i]);
	}
	printf("\n");
#endif	
	
	//���ܶ�ȡ�����ַ�������copy������ĵ�ַ�У�׼������ 
	decryKeyFileStr(keyfiledata, key);	
	 
	//����ֲ����� 
	for(i = 0; i < READ_SIZE; i++)
		keyfiledata[i] = 0;
	fd = 0;
	
	return 0;
}


int writeKeyFile(char * f_path, unsigned char * key){
	int i = 0;
	
	//���ܺ�ĵ�key_data
	unsigned char keyfiledata[READ_SIZE];
	//д�ļ��Ƿ�ɹ��ı�־���� 
	int readfileflag = 0;
	
	//���ļ���������� 
	int fd;
	fd = open(f_path, O_WRONLY);
	if(fd < 0)
	{
		printf("keyfilemanager.c\twriteKeyFile()\t*************************KEY FILE OPEN FAIL!*****************************\n");
		return -1;
	}
	
	//���ܴ���ĵ��ַ�������copy��Ҫд��ĵ�ַ�У�׼��д�� 
	decryKeyFileStr(key, keyfiledata);
	
	//д�ļ���������Ƿ���� 
	readfileflag = write(fd, keyfiledata, READ_SIZE);
	if(readfileflag <= 0){
		printf("keyfilemanager.c\twriteKeyFile()\t*************************KEY FILE WRITE ERROR!*****************************\n");
		return -1;
	}
	close(fd);
	
#ifdef DEBUG
	printf("keyfilemanager.c\twriteKeyFile()\t")	;
	for(i=0;i<READ_SIZE;i++)
	{
		printf("%02x",keyfiledata[i]);
	}
	printf("\n");
#endif		
	 
	//����ֲ����� 
	for(i = 0; i < READ_SIZE; i++)
		keyfiledata[i] = 0;
	fd = 0;
	
	return 0;
}





