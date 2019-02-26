#include "../mylock_config"

//为了实现数据+校验码组成数据单元
//在每个hash值后面加上1Byte的压码

void hash_add_checkcode(char * hash){
	int i, sum;
	sum = 0;
	for(i = 0; i < HASH_LEN; i++){
		sum = (sum + hash[i]) & 0x7F;
	}
	
	hash[HASH_LEN] = sum;
	
	return;
}

int check_hash_checkcode(char * hash){
	int i, sum;
	sum = 0;
	for(i = 0; i < HASH_LEN; i++){
		sum = (sum + hash[i]) & 0x7F;
	}
	
	if(hash[HASH_LEN] == sum)
		return 1;
	else
		return 0;
}

