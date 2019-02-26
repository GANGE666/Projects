#if 0

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include <conio.h>

typedef struct
{
	char name[50];//电梯名称
	int lc;//电梯所在楼层
	int rs;//电梯中的人数,规定最大乘坐人数为12
	int door;//门的状态 0关1开
}dianti;

typedef struct
{
	char name[50]; //这个人的名字或序号(以示区别)
	int rrsj;//一个人的容忍时间
	int lc;//所在楼层
	int md;//目的楼层
	int biaozhi; //此标志位记录人使用电梯是否完成或者是否离去,为1表明使用过(离去),否则为0
	int in;      //标志位,为1时表明人在电梯中,否则为0
	int cha;
}man;




typedef struct Node
{
	char a[1000];//事件信息
	int mytime;//事件所发生的时间
	struct Node *next;
}SLNode;

//初始化链表
void ListInitiate(SLNode* *head)
{
	if ((*head = (SLNode *)malloc(sizeof(SLNode))) == NULL) exit(1);
	(*head)->next = NULL;
}

//将事件结点插入到事件链表中的第i个位置中去
int ListInsert(SLNode *head, int i, char a[1000], int t)
{
	SLNode *p, *q;
	int j;
	p = head;
	j = -1;
	while (p->next != NULL && j<i - 1) {
		p = p->next;
		j++;
	}
	if (j != i - 1) {
		printf("\n            插入位置参数错!\n");
		return 0;
	}
	if ((q = (SLNode *)malloc(sizeof(SLNode))) == NULL) exit(1);
	q->mytime = t;
	strcpy(q->a, a);
	q->next = p->next;
	p->next = q;
	return 1;
}

//用按发生时间的顺序给事件排序
void LinListSort(SLNode *head)
{
	SLNode *curr, *pre, *p, *q;
	p = head->next;
	head->next = NULL;
	while (p != NULL)
	{
		curr = head->next;
		pre = head;
		while (curr != NULL && curr->mytime <= p->mytime) {
			pre = curr;
			curr = curr->next;
		}
		q = p;
		p = p->next;
		q->next = pre->next;
		pre->next = q;
	}
}

//销毁链表 
void Destroy(SLNode * *head)
{
	SLNode *p, *p1;
	p = *head;
	while (p != NULL) {
		p1 = p;
		p = p->next;
		free(p1);
	}
	*head = NULL;
}

/*
电梯动作： 1.开始开门,20t,开门结束;
2.开始关门,20t,关门结束;
3.从n-1层开始移动,30t,到达第n层;
4.开始等待,有人来或者300t,等待结束;
人的动作： 1.开始等待,x<=最大容忍时间,等待结束;
2.开始进门,25t,进门结束;
3.开始出门,25t,出门结束;
*/

int g = 0;

//电梯开门
void OPEN(SLNode *head, dianti dt, int now)
{
	char a[1000] = {  }, b[1000] = { "开门完毕!" }, temp3[10] = { "到达" }, temp[10], temp2[100] = { "层， 电梯开始开门......" };
	strcat(a, dt.name);
	strcat(a, temp3);
	itoa(dt.lc, temp, 10);
	strcat(a, temp);
	strcat(a, temp2);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 20);
	g++;
	dt.door = 1;                         //门的状态为开
}

//电梯关门
void CLOSE(SLNode *head, dianti dt, int now)
{
	char a[1000] = { }, b[1000] = { "关门完毕!" };
	char temp[50] = { "开始关门......" };
	strcat(a, dt.name);
	strcat(a, temp);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 20);
	g++;
	dt.door = 0;                        //门的状态为关
}

//人进电梯
void GET_IN(SLNode *head, dianti dt, int now, char name[])
{
	char a[1000] = {  }, b[1000] = { "已经进入" }, temp[50] = { "开始进入" };
	strcat(a, name);
	strcat(a, temp);
	strcat(a, dt.name);
	strcat(b, dt.name);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 25);
	g++;
	dt.rs++;                                 //电梯人数加一个
}

//人出电梯
void GET_OUT(SLNode *head, dianti dt, int now, char name[])
{
	char a[1000] = {  }, b[1000] = { "已经离开" }, temp[50] = { "开始离开" };
	strcat(a, name);
	strcat(a, temp);
	strcat(a, dt.name);
	strcat(b, dt.name);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 25);
	g++;
	dt.rs--;                                 //电梯中的人数减一个
}

void TIME_OUT(SLNode *head, dianti dt, int now, char name[]) {
	//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
	char a[1000] = { "超过用户的容忍时间,第" };
	char temp[10];
	itoa(dt.lc, temp, 10);
	strcat(a, temp);
	strcat(a, "层楼的");
	strcat(a, name);
	strcat(a, "已经离去");
	ListInsert(head, g, a, now);
	g++;
}

//返回最新事件（即事件链表最后一个结点）的发生时间
int gettime(SLNode *head)
{
	SLNode *pre, *curr;
	curr = head->next;
	pre = head;
	while (curr != NULL) {
		pre = curr;
		curr = curr->next;
	}
	return pre->mytime;
}

//电梯运行函数
void Go(SLNode *head, int n, dianti dt, clock_t s[1000], man r[1000])
{
	int k = 0, i, time, m, flag = 0, counter = 0;    //flag 记录是否有多个人在同一层或去同一层楼
	for (i = 0; i<n; i++) {
		if (dt.door == 0) {                                               //初始电梯门状态为关着	
			if (dt.lc == r[i].lc&&dt.rs<12 && r[i].biaozhi == 0 && r[i].in == 0) {//人在该楼层,要进去电梯,且电梯人数不超过规定的12人 
				if (i == 0)  time = s[i] / 100;                              //记录当时时间
				else time = gettime(head);
				if (flag == 0 && dt.door == 0) {
					OPEN(head, dt, time);       //开门
					dt.door = 1;
				}
				if ((time - s[i] / 100) <= r[i].rrsj) {
					time = gettime(head);
					GET_IN(head, dt, time, r[i].name);    //若超过此人的最大容忍时间,该人将放弃乘坐电梯
					r[i].in = 1;
				}
				else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
					r[m].biaozhi = 1;
					TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
					//printf("\n此时有人离开,以下为离开的人的信息:\n");
					//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
				}
				time = gettime(head);  //获得事件链表最后一个结点的时间(即当前时刻)
				if (dt.lc == r[i + 1].lc) { flag = 1; counter++; continue; } //该层的所有人都进入电梯
				else {
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);     //电梯经过40t时间后，若无人进出，则关门
						dt.door = 0;                                             //置电梯门状态为关
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				/* 本层人已经全部进入电梯，接下来电梯启动按每个人事件发生的时间顺序将其送往目的楼层*/
				flag = 0;
				if (dt.lc <= r[i - counter].md) {
					time = gettime(head);
					while (dt.lc <= r[i - counter].md) { //开往该层第一个人的目的楼层	
						flag = 0;
						if (dt.lc<r[i - counter].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {//若该楼层有人等候，使其进入		
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;                                   //电梯开门,并设置其门状态为开
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i - counter].md) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);     //电梯经过40t时间后，若无人进出，则关门
							dt.door = 0;                                             //置电梯门状态为关
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1)
					{
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				else if (dt.lc >= r[i - counter].md)
				{
					time = gettime(head);
					while (dt.lc >= r[i - counter].md) {
						flag = 0;
						if (dt.lc>r[i - counter].md) {
							dt.lc--;                    //30t,电梯下降一层
							time += 30;
						}
						for (m = 0; m<n; m++) {
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) { //若该楼层有人等候，使其进入	
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去~~\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i - counter].md)  break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				i = i - counter;
			}
			else if (dt.lc == r[i].md&&r[i].biaozhi == 0 && r[i].in == 1) { //人在电梯中,在该楼层要出去
				if (i == 0)time = s[0] / 100;
				else time = gettime(head);
				for (m = 0; m<n; m++) {
					if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) { //若该楼层有人等候，使其进入
						if (flag == 0 && dt.door == 0) {
							OPEN(head, dt, time);
							dt.door = 1;
						}
						if ((time - s[m] / 100) <= r[m].rrsj) {
							time = gettime(head);
							GET_IN(head, dt, time, r[m].name);
							r[m].in = 1;
						}
						else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
							r[m].biaozhi = 1;
							TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
							//printf("\n此时有人离开,以下为离开的人的信息:\n");
							//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
						}
						flag = 1;
					}
					//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
					else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
						if (flag == 0 && dt.door == 0) {
							OPEN(head, dt, time);
							dt.door = 1;
						}
						time = gettime(head);
						GET_OUT(head, dt, time, r[m].name);
						r[m].biaozhi = 1;
						r[m].in = 0;
						flag = 1;
					}
				}
				if (dt.door == 1) {
					time = gettime(head);
					ListInsert(head, g, "该层已无人进出,关门", time + 40);
					dt.door = 0;
					g++;
					CLOSE(head, dt, time + 40);
				}
			}
			//1...if(dt.lc==r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0) ,,,,人在该楼层,要进去电梯
			//2...else if(dt.lc==r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,人在电梯中,在该楼层要出去
			//3...else if(dt.lc!=r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0),,,,,,,,,,,人不在该楼层,要进去电梯
			//4...else if(dt.lc!=r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,,人在电梯里,该楼层不是其目的楼层
			else if (dt.lc != r[i].lc&&dt.rs<12 && r[i].biaozhi == 0 && r[i].in == 0) {   //3...人不在该楼层,要进去电梯
				flag = 0;
				if (i == 0)time = s[0] / 100;                  //以该事件发生的时间为起始时间
				else time = gettime(head);                //取当前时间                                           
				if (dt.lc <= r[i].lc) {
					while (dt.lc <= r[i].lc) {
						flag = 0;
						if (dt.lc<r[i].lc) {
							dt.lc++;
							time += 30;                                           //30t,电梯上升一层
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].lc) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
						time = gettime(head);
					}
				}
				else if (dt.lc >= r[i].lc) {
					while (dt.lc >= r[i].lc) {
						flag = 0;
						if (dt.lc>r[i].lc) {
							dt.lc--;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去~~\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].lc) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				/*接下来将电梯里的人按时间顺序送到各自的目的楼层*/
				time = gettime(head);
				if (dt.lc <= r[i].md&&r[i].in == 1) {
					while (dt.lc <= r[i].md) {
						flag = 0;
						if (dt.lc<r[i].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].md) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				else if (dt.lc >= r[i].md&&r[i].in == 1) {
					while (dt.lc >= r[i].md) {
						flag = 0;
						if (dt.lc>r[i].md) {
							dt.lc--;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].md) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
			}
			//1...if(dt.lc==r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0) ,,,,人在该楼层,要进去电梯
			//2...else if(dt.lc==r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,人在电梯中,在该楼层要出去
			//3...else if(dt.lc!=r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0),,,,,,,,,人不在该楼层,要进去电梯       
			else if (dt.lc != r[i].md&&r[i].biaozhi == 0 && r[i].in == 1) {  	//4...人在电梯里,该楼层不是其目的楼层
				flag = 0;
				if (i == 0)time = s[0] / 100;                        //以该事件发生的时间为起始时间
				else time = gettime(head);                      //取当前时间
				if (dt.lc <= r[i].md) {
					while (dt.lc <= r[i].md) {
						flag = 0;
						if (dt.lc<r[i].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)      \t超过用户的容忍时间,第%d层楼的人已经离去~~\n", s[m] / 100 + r[m].rrsj, dt.lc);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].md) break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				else if (dt.lc >= r[i].md) {
					while (dt.lc >= r[i].md) {
						flag = 0;
						if (dt.lc>r[i].md) {
							dt.lc--;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//若该楼层有人等候，使其进入
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n此时有人离开,以下为离开的人的信息:\n");
									//printf("时间:%d(单位:t)     \t超过用户的容忍时间,第%d层楼的%s已经离去\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//若该楼层有人要出去，使其出电梯，置其状态为1表示该人已经使用过电梯
							else if (dt.lc == r[m].md&&r[m].biaozhi == 0 && r[m].in == 1) {
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;
								}
								time = gettime(head);
								GET_OUT(head, dt, time, r[m].name);
								r[m].biaozhi = 1;
								r[m].in = 0;
								flag = 1;
							}
						}
						if (dt.lc == r[i].md)  break;
						if (dt.door == 1) {
							time = gettime(head);
							ListInsert(head, g, "该层已无人进出,关门", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "该层已无人进出,关门", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
			}
		}
	}
	time = gettime(head);
	if (dt.lc != 1) {
		char temp1[300] = {};
		/*
		strcat(temp1, dt.name);
		strcat(temp1, "在该层等候300t时间,无人进出，开始返回第一层");
		ListInsert(head, g, temp1, time + 300);
		g++;
		time = gettime(head);
		memset(temp1, 0, sizeof(temp1));
		*/
		strcat(temp1, dt.name);
		strcat(temp1, "已经回到第一层,等候使用......");
		if (dt.lc>1) { ListInsert(head, g, temp1, time + (dt.lc - 1) * 30); }
		else { ListInsert(head, g, temp1, time + 30); }
		dt.lc = 1;
	}
	else {
		char temp1[300] = {};
		strcat(temp1, dt.name);
		strcat(temp1, "已经在第一层,等候使用......");
		time = gettime(head);
		ListInsert(head, g, temp1, time);
	}
}

void print(SLNode *head, int fun)
{
	//fun = 1;详细模式
	char l[10][100] = { "开门完毕","关门完毕","该层","已经离开","开始关门","已经进入" };

	SLNode *q;
	q = head->next;
	while (q != NULL) {
		if (fun == 0 && (strstr(q->a, l[0]) != 0 || strstr(q->a, l[1]) != 0 || strstr(q->a, l[2]) != 0 || strstr(q->a, l[3]) != 0 || strstr(q->a, l[4]) != 0 || strstr(q->a, l[5]) != 0))
			printf("");
		else
			printf("\n时间:%d(单位:t)     \t%s", q->mytime, q->a);
		q = q->next;
	}
}

//欢迎界面
void Welcome()
{
	char c;
	printf("\n\n\n ******************************************************************************\n");
	printf(" \n\n\n\n\n");
	printf("                       欢迎使用电梯模拟调度程序                            \n");
	printf("                                制作者                    \n");
	printf("                       肖轩淦             2016301500327                     \n");
	printf("                       邹一鸣             2016301500324                     \n");
	printf("                       郭宏宇             2016301500325                     \n");
	printf("                       田欢               2016301500326                     \n");
	printf("                       阿合玛拉丽·叶尔肯 2016301500290                     \n");
	printf("\n\n\n\n\n");
	printf(" ******************************************************************************\n\n\n");
	printf("                                             按任意键继续\n");
	c = getchar();
	system("cls");
}

//将等待队列分为两个
int  fenpei(int num, man ren[], clock_t shijian[], man * &dt1, int &dt1ptr , clock_t shijian1[], man * &dt2, int &dt2ptr, clock_t shijian2[]) {
	int jilu1, jilu2, old;
	dt1ptr = 0;
	dt2ptr = 0;
	int biaoji1 = 0, biaoji2 = 0;//小于0表示向下 大于0表示向上
	for (int z = 0; z <= num - 1; z++) {
		ren[z].cha = ren[z].md - ren[z].lc;
	}
	for (int i = 0; i <= num - 1; i++) {
		jilu1 = dt1ptr;
		jilu2 = dt2ptr;
		if (i == 0) {
			shijian1[dt1ptr] = shijian[i];
			dt1[dt1ptr++] = ren[i];
			biaoji1 = ren[i].cha;
			old = 2;
		}
		else {
			if (biaoji1>0) {
				if (ren[i].cha*biaoji1>0 && ren[i].lc >= dt1[dt1ptr - 1].md) {
					shijian1[dt1ptr] = shijian[i];
					dt1[dt1ptr++] = ren[i];
				}
			}
			else {
				if (biaoji1*ren[i].cha>0 && ren[i].lc <= dt1[dt1ptr - 1].md) {
					shijian1[dt1ptr] = shijian[i];
					dt1[dt1ptr++] = ren[i];
				}
			}
			if (dt1ptr>jilu1) {
				old = 2;
				continue;
			}
			if (dt1ptr == jilu1&&dt2ptr == 0) {
				shijian2[dt2ptr] = shijian[i];
				dt2[dt2ptr++] = ren[i];
				continue;
			}


			if (biaoji2>0) {
				if (ren[i].cha*biaoji2>0 && ren[i].lc >= dt2[dt2ptr - 1].md) {
					shijian2[dt2ptr] = shijian[i];
					dt2[dt2ptr++] = ren[i];
				}
			}
			else {
				if (biaoji2*ren[i].cha>0 && ren[i].lc <= dt2[dt2ptr - 1].md) {
					shijian2[dt2ptr] = shijian[i];
					dt2[dt2ptr++] = ren[i];
				}
			}
			if (jilu2<dt2ptr) old = 1;


			if (jilu1 == dt1ptr&&jilu2 == dt2ptr) {
				if (old == 1) {
					shijian1[dt1ptr] = shijian[i];
					dt1[dt1ptr++] = ren[i];
					biaoji1 = ren[i].cha;
					old = 2;
				}
				else {
					shijian2[dt2ptr] = shijian[i];
					dt2[dt2ptr++] = ren[i];
					biaoji2 = ren[i].cha;
					old = 1;
				}
			}
		}
	}
	return 0;
}
//主函数

int main()
{
	int t, a, k = 0, i, flag = 0;
	man ren[1000];
	man * ren1, * ren2;
	ren1 = (man *)malloc(1000 * sizeof(man));
	ren2 = (man *)malloc(1000 * sizeof(man));

	SLNode *head;
	//数组,用来存输入数据时候所用时间
	clock_t start, shijian[1000], In_put[1000][2];//为了保证输入数据时计时暂停，定义一个In_put[]                                  
	dianti dt1, dt2;
	Welcome();
	printf("\n\n时间单位t:0.1(秒)");
	printf("\n楼层范围:0--8(层)，0层即为-1层");
	printf("\n最大载客量:12(人)\n");
	printf("电梯动作：\n	1.开始开门, 20t, 开门结束;\n	2.开始关门, 20t, 关门结束;\n	3.从n - 1层开始移动, 30t, 到达第n层;\n	4.开始等待, 有人来或者300t, 等待结束;\n人的动作：\n	1.开始等待, x <= 最大容忍时间, 等待结束;\n	2.开始进门, 25t, 进门结束;\n	3.开始出门, 25t, 出门结束;\n");
	dt1.lc = 1;                   //开始电梯停在第一层   
	dt2.lc = 1;
	dt1.rs = 0;                   //电梯最大人数为12,开始时候电梯内人数为空
	dt2.rs = 0;
	dt1.door = 0;
	dt2.door = 0;
	char temp[50] = { "电梯1" }, temp2[50] = { "电梯2" };
	strcpy(dt1.name, temp);
	strcpy(dt2.name, temp2);

	start = clock();
	t = (int)start / 100;
	printf("\n当前时刻：%d(t)\n", t);
	ListInitiate(&head);
	while (1) {
		printf("\n0.结束输入 1.输入乘客信息\n");
		printf("请选择指令：");
		scanf("%d", &a);
		if (a == 0) { printf("\n"); break; }
		if (a == 1) {
			In_put[k][0] = clock();
			printf("\n请输入此人的名称(可以为任意字符): ");
			scanf("%s", ren[k].name);
			printf("所在楼层: ");
			scanf("%d", &ren[k].lc);
			printf("目的楼层: ");
			scanf("%d", &ren[k].md);
			printf("最大容忍时间:");
			scanf("%d", &ren[k].rrsj);
			In_put[k][1] = clock();
			k++;
		}
	}
	/*
	printf("------------------------------------------------------------------------------");
	printf("\n以下为要使用电梯的各个人的信息:(根据其申请使用电梯的时间先后顺序排列)\n");
	for (i = 0; i<k; i++) {
		int j;
		//flag = 0;
		for (j = 0; j<i; j++) {
			if (ren[i].lc == ren[j].lc) { flag = 1; break; }          //判断这层是否有多个人一起等候电梯
		}
		
		/*
		if (flag == 1)              //若有多个人一起等候电梯,则申请电梯的时间取第一个人的时间 
			printf("时间: %d(单位:t)  人序号:%s 所在楼层:%d   要去楼层:%d\n", shijian[j] / 100, ren[i].name, ren[i].lc, ren[i].md);
		else if (flag == 0)
		*/
	/*
			printf("时间: %d(单位:t)  人序号:%s 所在楼层:%d   要去楼层:%d   最大容忍时间:%d\n", shijian[j] / 100, ren[i].name, ren[i].lc, ren[i].md, ren[i].rrsj);
	}
	printf("\n------------------------------------------------------------------------------\n");
	*/
	
	for (i = 0; i<k; i++) {

		shijian[i] = In_put[i][0] - start;
		start = In_put[i][1] - In_put[i][0];              //计算输入输入数据所用时间

		ren[i].biaozhi = 0;
		ren[i].in = 0;
	}
	shijian[i] = 0;

	int k1, k2;
	clock_t shijian1[1000], shijian2[1000];
	fenpei(k, ren, shijian, ren1, k1, shijian1, ren2, k2, shijian2);

	SLNode * head1, * head2;
	ListInitiate(&head1);
	ListInitiate(&head2);


	//Go(head1, k, dt1, shijian, ren);
	Go(head1, k1, dt1, shijian1, ren1);
	g = 0;
	Go(head2, k2, dt2, shijian2, ren2);

	//合并事件列表
	head = head1;
	SLNode * p;
	ListInitiate(&p);
	p = head;
	while (p->next != NULL)
		p = p->next;
	p->next = head2->next;


	LinListSort(head);
	printf("\n---------------------------------------------------------------------------\n");
	printf("\n事件：");
	print(head, 0);
	printf("\n\n---------------------------------------------------------------------------\n\n");
	printf("按下回车查看详细事件\n");

	char c;
	while ((c = getchar()) != EOF && c != '\n');
	getchar();

	
	printf("\n---------------------------------------------------------------------------\n");
	printf("\n详细事件：");
	print(head, 1);
	printf("\n\n---------------------------------------------------------------------------\n\n");


	Destroy(&head);
	system("pause");
	return 0;
}
/*
测试数据：
1
乘客1
2
4
299
1
乘客2
5
7
399
1
乘客3
2
3
399
1
乘客4
6
5
399
1
乘客5
4
7
499
1
乘客6
4
3
799
1
乘客7
3
1
899
1
乘客8
7
8
899
0

平均等待时间：
(254+579+281+721+444+576+441+794)/8-27（初始时间）=484
空载率：
(2+1+3+2+3+1)/(7+3+3+1+6+1+2+2)=48%
电梯1：(2+1+3)/(7+3+3+1)=42.85%
电梯2：(2+3+1)/(6+1+2+2)=54.54%

*/

#endif