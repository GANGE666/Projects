#if 0

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include <conio.h>

typedef struct
{
	char name[50];//��������
	int lc;//��������¥��
	int rs;//�����е�����,�涨����������Ϊ12
	int door;//�ŵ�״̬ 0��1��
}dianti;

typedef struct
{
	char name[50]; //����˵����ֻ����(��ʾ����)
	int rrsj;//һ���˵�����ʱ��
	int lc;//����¥��
	int md;//Ŀ��¥��
	int biaozhi; //�˱�־λ��¼��ʹ�õ����Ƿ���ɻ����Ƿ���ȥ,Ϊ1����ʹ�ù�(��ȥ),����Ϊ0
	int in;      //��־λ,Ϊ1ʱ�������ڵ�����,����Ϊ0
	int cha;
}man;




typedef struct Node
{
	char a[1000];//�¼���Ϣ
	int mytime;//�¼���������ʱ��
	struct Node *next;
}SLNode;

//��ʼ������
void ListInitiate(SLNode* *head)
{
	if ((*head = (SLNode *)malloc(sizeof(SLNode))) == NULL) exit(1);
	(*head)->next = NULL;
}

//���¼������뵽�¼������еĵ�i��λ����ȥ
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
		printf("\n            ����λ�ò�����!\n");
		return 0;
	}
	if ((q = (SLNode *)malloc(sizeof(SLNode))) == NULL) exit(1);
	q->mytime = t;
	strcpy(q->a, a);
	q->next = p->next;
	p->next = q;
	return 1;
}

//�ð�����ʱ���˳����¼�����
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

//�������� 
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
���ݶ����� 1.��ʼ����,20t,���Ž���;
2.��ʼ����,20t,���Ž���;
3.��n-1�㿪ʼ�ƶ�,30t,�����n��;
4.��ʼ�ȴ�,����������300t,�ȴ�����;
�˵Ķ����� 1.��ʼ�ȴ�,x<=�������ʱ��,�ȴ�����;
2.��ʼ����,25t,���Ž���;
3.��ʼ����,25t,���Ž���;
*/

int g = 0;

//���ݿ���
void OPEN(SLNode *head, dianti dt, int now)
{
	char a[1000] = {  }, b[1000] = { "�������!" }, temp3[10] = { "����" }, temp[10], temp2[100] = { "�㣬 ���ݿ�ʼ����......" };
	strcat(a, dt.name);
	strcat(a, temp3);
	itoa(dt.lc, temp, 10);
	strcat(a, temp);
	strcat(a, temp2);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 20);
	g++;
	dt.door = 1;                         //�ŵ�״̬Ϊ��
}

//���ݹ���
void CLOSE(SLNode *head, dianti dt, int now)
{
	char a[1000] = { }, b[1000] = { "�������!" };
	char temp[50] = { "��ʼ����......" };
	strcat(a, dt.name);
	strcat(a, temp);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 20);
	g++;
	dt.door = 0;                        //�ŵ�״̬Ϊ��
}

//�˽�����
void GET_IN(SLNode *head, dianti dt, int now, char name[])
{
	char a[1000] = {  }, b[1000] = { "�Ѿ�����" }, temp[50] = { "��ʼ����" };
	strcat(a, name);
	strcat(a, temp);
	strcat(a, dt.name);
	strcat(b, dt.name);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 25);
	g++;
	dt.rs++;                                 //����������һ��
}

//�˳�����
void GET_OUT(SLNode *head, dianti dt, int now, char name[])
{
	char a[1000] = {  }, b[1000] = { "�Ѿ��뿪" }, temp[50] = { "��ʼ�뿪" };
	strcat(a, name);
	strcat(a, temp);
	strcat(a, dt.name);
	strcat(b, dt.name);

	ListInsert(head, g, a, now);
	g++;
	ListInsert(head, g, b, now + 25);
	g++;
	dt.rs--;                                 //�����е�������һ��
}

void TIME_OUT(SLNode *head, dianti dt, int now, char name[]) {
	//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
	char a[1000] = { "�����û�������ʱ��,��" };
	char temp[10];
	itoa(dt.lc, temp, 10);
	strcat(a, temp);
	strcat(a, "��¥��");
	strcat(a, name);
	strcat(a, "�Ѿ���ȥ");
	ListInsert(head, g, a, now);
	g++;
}

//���������¼������¼��������һ����㣩�ķ���ʱ��
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

//�������к���
void Go(SLNode *head, int n, dianti dt, clock_t s[1000], man r[1000])
{
	int k = 0, i, time, m, flag = 0, counter = 0;    //flag ��¼�Ƿ��ж������ͬһ���ȥͬһ��¥
	for (i = 0; i<n; i++) {
		if (dt.door == 0) {                                               //��ʼ������״̬Ϊ����	
			if (dt.lc == r[i].lc&&dt.rs<12 && r[i].biaozhi == 0 && r[i].in == 0) {//���ڸ�¥��,Ҫ��ȥ����,�ҵ��������������涨��12�� 
				if (i == 0)  time = s[i] / 100;                              //��¼��ʱʱ��
				else time = gettime(head);
				if (flag == 0 && dt.door == 0) {
					OPEN(head, dt, time);       //����
					dt.door = 1;
				}
				if ((time - s[i] / 100) <= r[i].rrsj) {
					time = gettime(head);
					GET_IN(head, dt, time, r[i].name);    //���������˵��������ʱ��,���˽�������������
					r[i].in = 1;
				}
				else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
					r[m].biaozhi = 1;
					TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
					//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
					//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
				}
				time = gettime(head);  //����¼��������һ������ʱ��(����ǰʱ��)
				if (dt.lc == r[i + 1].lc) { flag = 1; counter++; continue; } //�ò�������˶��������
				else {
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);     //���ݾ���40tʱ��������˽����������
						dt.door = 0;                                             //�õ�����״̬Ϊ��
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				/* �������Ѿ�ȫ��������ݣ�����������������ÿ�����¼�������ʱ��˳��������Ŀ��¥��*/
				flag = 0;
				if (dt.lc <= r[i - counter].md) {
					time = gettime(head);
					while (dt.lc <= r[i - counter].md) { //�����ò��һ���˵�Ŀ��¥��	
						flag = 0;
						if (dt.lc<r[i - counter].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) {//����¥�����˵Ⱥ�ʹ�����		
								if (flag == 0 && dt.door == 0) {
									OPEN(head, dt, time);
									dt.door = 1;                                   //���ݿ���,����������״̬Ϊ��
								}
								if ((time - s[m] / 100) <= r[m].rrsj) {
									time = gettime(head);
									GET_IN(head, dt, time, r[m].name);
									r[m].in = 1;
								}
								else if (((time - s[m] / 100)>r[m].rrsj) && r[m].biaozhi == 0) {
									r[m].biaozhi = 1;
									TIME_OUT(head, dt, s[m] / 100 + r[m].rrsj, r[m].name);
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);     //���ݾ���40tʱ��������˽����������
							dt.door = 0;                                             //�õ�����״̬Ϊ��
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1)
					{
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
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
							dt.lc--;                    //30t,�����½�һ��
							time += 30;
						}
						for (m = 0; m<n; m++) {
							if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) { //����¥�����˵Ⱥ�ʹ�����	
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ~~\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				i = i - counter;
			}
			else if (dt.lc == r[i].md&&r[i].biaozhi == 0 && r[i].in == 1) { //���ڵ�����,�ڸ�¥��Ҫ��ȥ
				if (i == 0)time = s[0] / 100;
				else time = gettime(head);
				for (m = 0; m<n; m++) {
					if (dt.lc == r[m].lc&&r[m].biaozhi == 0 && r[m].in == 0 && dt.rs<12) { //����¥�����˵Ⱥ�ʹ�����
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
							//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
							//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
						}
						flag = 1;
					}
					//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
					ListInsert(head, g, "�ò������˽���,����", time + 40);
					dt.door = 0;
					g++;
					CLOSE(head, dt, time + 40);
				}
			}
			//1...if(dt.lc==r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0) ,,,,���ڸ�¥��,Ҫ��ȥ����
			//2...else if(dt.lc==r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,���ڵ�����,�ڸ�¥��Ҫ��ȥ
			//3...else if(dt.lc!=r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0),,,,,,,,,,,�˲��ڸ�¥��,Ҫ��ȥ����
			//4...else if(dt.lc!=r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,,���ڵ�����,��¥�㲻����Ŀ��¥��
			else if (dt.lc != r[i].lc&&dt.rs<12 && r[i].biaozhi == 0 && r[i].in == 0) {   //3...�˲��ڸ�¥��,Ҫ��ȥ����
				flag = 0;
				if (i == 0)time = s[0] / 100;                  //�Ը��¼�������ʱ��Ϊ��ʼʱ��
				else time = gettime(head);                //ȡ��ǰʱ��                                           
				if (dt.lc <= r[i].lc) {
					while (dt.lc <= r[i].lc) {
						flag = 0;
						if (dt.lc<r[i].lc) {
							dt.lc++;
							time += 30;                                           //30t,��������һ��
						}
						for (m = 0; m<n; m++) {
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
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
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ~~\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
				/*����������������˰�ʱ��˳���͵����Ե�Ŀ��¥��*/
				time = gettime(head);
				if (dt.lc <= r[i].md&&r[i].in == 1) {
					while (dt.lc <= r[i].md) {
						flag = 0;
						if (dt.lc<r[i].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
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
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
						dt.door = 0;
						g++;
						CLOSE(head, dt, time + 40);
					}
				}
			}
			//1...if(dt.lc==r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0) ,,,,���ڸ�¥��,Ҫ��ȥ����
			//2...else if(dt.lc==r[i].md&&r[i].biaozhi==0&&r[i].in==1),,,,,,,,,,���ڵ�����,�ڸ�¥��Ҫ��ȥ
			//3...else if(dt.lc!=r[i].lc&&dt.rs<12&&r[i].biaozhi==0&&r[i].in==0),,,,,,,,,�˲��ڸ�¥��,Ҫ��ȥ����       
			else if (dt.lc != r[i].md&&r[i].biaozhi == 0 && r[i].in == 1) {  	//4...���ڵ�����,��¥�㲻����Ŀ��¥��
				flag = 0;
				if (i == 0)time = s[0] / 100;                        //�Ը��¼�������ʱ��Ϊ��ʼʱ��
				else time = gettime(head);                      //ȡ��ǰʱ��
				if (dt.lc <= r[i].md) {
					while (dt.lc <= r[i].md) {
						flag = 0;
						if (dt.lc<r[i].md) {
							dt.lc++;
							time += 30;
						}
						for (m = 0; m<n; m++) {
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)      \t�����û�������ʱ��,��%d��¥�����Ѿ���ȥ~~\n", s[m] / 100 + r[m].rrsj, dt.lc);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
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
							//����¥�����˵Ⱥ�ʹ�����
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
									//printf("\n��ʱ�����뿪,����Ϊ�뿪���˵���Ϣ:\n");
									//printf("ʱ��:%d(��λ:t)     \t�����û�������ʱ��,��%d��¥��%s�Ѿ���ȥ\n", s[m] / 100 + r[m].rrsj, dt.lc, r[m].name);
								}
								flag = 1;
							}
							//����¥������Ҫ��ȥ��ʹ������ݣ�����״̬Ϊ1��ʾ�����Ѿ�ʹ�ù�����
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
							ListInsert(head, g, "�ò������˽���,����", time + 40);
							dt.door = 0;
							g++;
							CLOSE(head, dt, time + 40);
							time = gettime(head);
						}
					}
					if (dt.door == 1) {
						time = gettime(head);
						ListInsert(head, g, "�ò������˽���,����", time + 40);
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
		strcat(temp1, "�ڸò�Ⱥ�300tʱ��,���˽�������ʼ���ص�һ��");
		ListInsert(head, g, temp1, time + 300);
		g++;
		time = gettime(head);
		memset(temp1, 0, sizeof(temp1));
		*/
		strcat(temp1, dt.name);
		strcat(temp1, "�Ѿ��ص���һ��,�Ⱥ�ʹ��......");
		if (dt.lc>1) { ListInsert(head, g, temp1, time + (dt.lc - 1) * 30); }
		else { ListInsert(head, g, temp1, time + 30); }
		dt.lc = 1;
	}
	else {
		char temp1[300] = {};
		strcat(temp1, dt.name);
		strcat(temp1, "�Ѿ��ڵ�һ��,�Ⱥ�ʹ��......");
		time = gettime(head);
		ListInsert(head, g, temp1, time);
	}
}

void print(SLNode *head, int fun)
{
	//fun = 1;��ϸģʽ
	char l[10][100] = { "�������","�������","�ò�","�Ѿ��뿪","��ʼ����","�Ѿ�����" };

	SLNode *q;
	q = head->next;
	while (q != NULL) {
		if (fun == 0 && (strstr(q->a, l[0]) != 0 || strstr(q->a, l[1]) != 0 || strstr(q->a, l[2]) != 0 || strstr(q->a, l[3]) != 0 || strstr(q->a, l[4]) != 0 || strstr(q->a, l[5]) != 0))
			printf("");
		else
			printf("\nʱ��:%d(��λ:t)     \t%s", q->mytime, q->a);
		q = q->next;
	}
}

//��ӭ����
void Welcome()
{
	char c;
	printf("\n\n\n ******************************************************************************\n");
	printf(" \n\n\n\n\n");
	printf("                       ��ӭʹ�õ���ģ����ȳ���                            \n");
	printf("                                ������                    \n");
	printf("                       Ф����             2016301500327                     \n");
	printf("                       ��һ��             2016301500324                     \n");
	printf("                       ������             2016301500325                     \n");
	printf("                       �ﻶ               2016301500326                     \n");
	printf("                       ������������Ҷ���� 2016301500290                     \n");
	printf("\n\n\n\n\n");
	printf(" ******************************************************************************\n\n\n");
	printf("                                             �����������\n");
	c = getchar();
	system("cls");
}

//���ȴ����з�Ϊ����
int  fenpei(int num, man ren[], clock_t shijian[], man * &dt1, int &dt1ptr , clock_t shijian1[], man * &dt2, int &dt2ptr, clock_t shijian2[]) {
	int jilu1, jilu2, old;
	dt1ptr = 0;
	dt2ptr = 0;
	int biaoji1 = 0, biaoji2 = 0;//С��0��ʾ���� ����0��ʾ����
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
//������

int main()
{
	int t, a, k = 0, i, flag = 0;
	man ren[1000];
	man * ren1, * ren2;
	ren1 = (man *)malloc(1000 * sizeof(man));
	ren2 = (man *)malloc(1000 * sizeof(man));

	SLNode *head;
	//����,��������������ʱ������ʱ��
	clock_t start, shijian[1000], In_put[1000][2];//Ϊ�˱�֤��������ʱ��ʱ��ͣ������һ��In_put[]                                  
	dianti dt1, dt2;
	Welcome();
	printf("\n\nʱ�䵥λt:0.1(��)");
	printf("\n¥�㷶Χ:0--8(��)��0�㼴Ϊ-1��");
	printf("\n����ؿ���:12(��)\n");
	printf("���ݶ�����\n	1.��ʼ����, 20t, ���Ž���;\n	2.��ʼ����, 20t, ���Ž���;\n	3.��n - 1�㿪ʼ�ƶ�, 30t, �����n��;\n	4.��ʼ�ȴ�, ����������300t, �ȴ�����;\n�˵Ķ�����\n	1.��ʼ�ȴ�, x <= �������ʱ��, �ȴ�����;\n	2.��ʼ����, 25t, ���Ž���;\n	3.��ʼ����, 25t, ���Ž���;\n");
	dt1.lc = 1;                   //��ʼ����ͣ�ڵ�һ��   
	dt2.lc = 1;
	dt1.rs = 0;                   //�����������Ϊ12,��ʼʱ�����������Ϊ��
	dt2.rs = 0;
	dt1.door = 0;
	dt2.door = 0;
	char temp[50] = { "����1" }, temp2[50] = { "����2" };
	strcpy(dt1.name, temp);
	strcpy(dt2.name, temp2);

	start = clock();
	t = (int)start / 100;
	printf("\n��ǰʱ�̣�%d(t)\n", t);
	ListInitiate(&head);
	while (1) {
		printf("\n0.�������� 1.����˿���Ϣ\n");
		printf("��ѡ��ָ�");
		scanf("%d", &a);
		if (a == 0) { printf("\n"); break; }
		if (a == 1) {
			In_put[k][0] = clock();
			printf("\n��������˵�����(����Ϊ�����ַ�): ");
			scanf("%s", ren[k].name);
			printf("����¥��: ");
			scanf("%d", &ren[k].lc);
			printf("Ŀ��¥��: ");
			scanf("%d", &ren[k].md);
			printf("�������ʱ��:");
			scanf("%d", &ren[k].rrsj);
			In_put[k][1] = clock();
			k++;
		}
	}
	/*
	printf("------------------------------------------------------------------------------");
	printf("\n����ΪҪʹ�õ��ݵĸ����˵���Ϣ:(����������ʹ�õ��ݵ�ʱ���Ⱥ�˳������)\n");
	for (i = 0; i<k; i++) {
		int j;
		//flag = 0;
		for (j = 0; j<i; j++) {
			if (ren[i].lc == ren[j].lc) { flag = 1; break; }          //�ж�����Ƿ��ж����һ��Ⱥ����
		}
		
		/*
		if (flag == 1)              //���ж����һ��Ⱥ����,��������ݵ�ʱ��ȡ��һ���˵�ʱ�� 
			printf("ʱ��: %d(��λ:t)  �����:%s ����¥��:%d   Ҫȥ¥��:%d\n", shijian[j] / 100, ren[i].name, ren[i].lc, ren[i].md);
		else if (flag == 0)
		*/
	/*
			printf("ʱ��: %d(��λ:t)  �����:%s ����¥��:%d   Ҫȥ¥��:%d   �������ʱ��:%d\n", shijian[j] / 100, ren[i].name, ren[i].lc, ren[i].md, ren[i].rrsj);
	}
	printf("\n------------------------------------------------------------------------------\n");
	*/
	
	for (i = 0; i<k; i++) {

		shijian[i] = In_put[i][0] - start;
		start = In_put[i][1] - In_put[i][0];              //��������������������ʱ��

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

	//�ϲ��¼��б�
	head = head1;
	SLNode * p;
	ListInitiate(&p);
	p = head;
	while (p->next != NULL)
		p = p->next;
	p->next = head2->next;


	LinListSort(head);
	printf("\n---------------------------------------------------------------------------\n");
	printf("\n�¼���");
	print(head, 0);
	printf("\n\n---------------------------------------------------------------------------\n\n");
	printf("���»س��鿴��ϸ�¼�\n");

	char c;
	while ((c = getchar()) != EOF && c != '\n');
	getchar();

	
	printf("\n---------------------------------------------------------------------------\n");
	printf("\n��ϸ�¼���");
	print(head, 1);
	printf("\n\n---------------------------------------------------------------------------\n\n");


	Destroy(&head);
	system("pause");
	return 0;
}
/*
�������ݣ�
1
�˿�1
2
4
299
1
�˿�2
5
7
399
1
�˿�3
2
3
399
1
�˿�4
6
5
399
1
�˿�5
4
7
499
1
�˿�6
4
3
799
1
�˿�7
3
1
899
1
�˿�8
7
8
899
0

ƽ���ȴ�ʱ�䣺
(254+579+281+721+444+576+441+794)/8-27����ʼʱ�䣩=484
�����ʣ�
(2+1+3+2+3+1)/(7+3+3+1+6+1+2+2)=48%
����1��(2+1+3)/(7+3+3+1)=42.85%
����2��(2+3+1)/(6+1+2+2)=54.54%

*/

#endif