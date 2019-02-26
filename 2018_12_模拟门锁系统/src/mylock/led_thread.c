#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pthread.h"




#define BUFFER_SIZE 16

/* Circular buffer of integers. */
struct prodcons {
  int buffer[BUFFER_SIZE];      /* the actual data */
  pthread_mutex_t lock;         /* mutex ensuring exclusive access to buffer */
  int readpos, writepos;        /* positions for reading and writing */
  pthread_cond_t notempty;      /* signaled when buffer is not empty */
  pthread_cond_t notfull;       /* signaled when buffer is not full */
};

/*--------------------------------------------------------*/
/* Initialize a buffer */
void thread_buff_init(struct prodcons * b)
{
  pthread_mutex_init(&b->lock, NULL);
  pthread_cond_init(&b->notempty, NULL);
  pthread_cond_init(&b->notfull, NULL);
  b->readpos = 0;
  b->writepos = 0;
}
/*--------------------------------------------------------*/
/* Store an integer in the buffer */
void buff_put(struct prodcons * b, int data)
{
	pthread_mutex_lock(&b->lock);

  	
  
  	b->buffer[0] = data;

  /* Signal that the buffer is now not empty */
  	pthread_cond_signal(&b->notempty);

	pthread_mutex_unlock(&b->lock);
}
/*--------------------------------------------------------*/
/* Read and remove an integer from the buffer */
int buff_get(struct prodcons * b)
{
  	int data;
    pthread_mutex_lock(&b->lock);

  	data = b->buffer[0];
  	b->buffer[0] = 0;
  	
  	/* Signal that the buffer is now not full */
  	pthread_cond_signal(&b->notfull);

  	pthread_mutex_unlock(&b->lock);
  	return data;
}
/*--------------------------------------------------------*/
#define OVER (-1)
struct prodcons buffer;
/*--------------------------------------------------------*/

void * deliver_to_led(void * data)
{
	int d;

	//led init
	if(led_init() == 0)
		printf("[INFO]\tLED INIT SUCC!\n");
	else
		printf("[ERROR]\tLED INIT ERROR!\n");

	while(1){
		d = buff_get(&buffer);

		//thread线程 传递给led
		led_control(d);
	}
	return NULL;
}


// main 中控制led，可使用wrtie_buff() 将控制信息传入缓冲区
void write_buff(int data)
{
	buff_put(&buffer, data);

	return;
}


// 调用led_thread_begin 开启led线程
void led_thread_begin(void)
{
	pthread_t th_led;
	void * retval;

	thread_buff_init(&buffer);

	pthread_create(&th_led, NULL, deliver_to_led, 0);

	return;
}














