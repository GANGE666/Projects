//返回0正常，-1错误


int state_roolback(int * current_state, int target_state){
	
	//只有当目标状态小于当前状态时才能回退
	if(target_state < *current_state){
		* current_state = target_state;
		return 0;
	}
	
	error_handel();
	return -1;	
}

int add_begin_state(int * current_state, int begin_state){
	* current_state |= begin_state;
	return 0;
}

int remove_one_state(int * current_state, int remove_state){
	* current_state &= (~remove_state);
	return 0;
}


//传入当前状态序列，要求序列，当前状态的二进制位
int check_update_state(int * current_state, int request_state, int current_state_bit){
	
	//检查
	if(*current_state >= request_state)
	{
		*current_state |= current_state_bit;
		return 0;
	}
	
	error_handel();
	return -1;
}