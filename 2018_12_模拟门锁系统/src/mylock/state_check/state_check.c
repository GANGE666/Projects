//����0������-1����


int state_roolback(int * current_state, int target_state){
	
	//ֻ�е�Ŀ��״̬С�ڵ�ǰ״̬ʱ���ܻ���
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


//���뵱ǰ״̬���У�Ҫ�����У���ǰ״̬�Ķ�����λ
int check_update_state(int * current_state, int request_state, int current_state_bit){
	
	//���
	if(*current_state >= request_state)
	{
		*current_state |= current_state_bit;
		return 0;
	}
	
	error_handel();
	return -1;
}