#include<stdlib.h>
#include<sys/time.h>

#define random(x) (rand()%x)

void random_init(){
	srand((int)time(0));	
	
	return;
}


// ���з�Χ����������㣬ʹÿ�εĽ��ܶԱȵ���������ͬ
void random_clac(){
	int i, j, k;
	
	j = 0;
	k = 0;
	
	for(i = 0; i < random(100)+20; i++)
		j += random(1024) ^ random(1024);
	
	return;
}


void random_md5(){
	int i,j,k;

	j = 0;
	k = 0;

	for(i = 0; i < random(10); i++)
		j = random(0xFFFFFFFF) ^ random(0xFFFFFFFF);

	return;
}

