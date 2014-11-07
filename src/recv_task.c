#include "../inc/down_types.h"
#include "../inc/recv_task.h"
//#include "../inc/down_task.h"

#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define DOWN_TASK_DEBUG 1
void *down_task(void *url);

void *recv_msg(void *arg)
{
	long int msgtype = 0;
	msg_web_cache_t msg;
	msg_back_web_cache_t msg_back;
	msgid_t *msgid = (msgid_t *)arg;
	int *flag;
	void **record;
	unsigned char *url;

#ifdef DOWN_TASK_DEBUG
	FILE *fp;
	fp = fopen("recv.txt", "wb");
	if(fp==NULL)
	{
		fprintf(stderr, "open file error!\n");
		exit(EXIT_FAILURE);
	}
#endif

	pthread_t t1;
	int s;
	void *res;
	while(1)
	{
		if(msgrcv(msgid->msgid_recv, (void *)&msg, MSG_MAX_SIZE, msgtype, 0)==-1)
		{
			fprintf(stderr, "msgrcv failed with error:%d\n", errno);
		}
		else
		{
			flag = (int *)(msg.mtext);
			record = (void **)(&msg.mtext[4]);
			int i;
			for(i=4; i<12; i++)
			{
				printf("%02x", msg.mtext[i]);
			}
			printf("\n");
			url = (char *)(&msg.mtext[12]);
			printf("%s\n", url);

#ifdef DOWN_TASK_DEBUG
			fprintf(fp,"%s\n", url);
#endif
			s = pthread_create(&t1, NULL, down_task, url);
			if(s!=0)
			{
				fprintf(stderr, "pthread_create error\n");
			}
			s = pthread_join(t1, &res);
			if(s!=0)
			{
				fprintf(stderr, "pthread_join error\n");
			}
			printf("thread returned %ld\n", (int)res);
			if((int)res==0)
			{
				//TODO：下载完成
				msg_back.msgtype = 1;
				*flag = 3;
//				strncpy(msg_back.mtext, msg.mtext, 12);
				int i;
				for(i=0; i<12; i++)
				{
					//printf("%02x", msg_back.mtext[i]);
					msg_back.mtext[i] = msg.mtext[i];
				}
				printf("\n");
				//在这里把msg_back回传给query模块
				if(msgsnd(msgid->msgid_reply, (void *)&msg_back, MSG_BACK_MAX_SIZE, 0)==-1)
				{
					fprintf(stderr, "msgsnd error: %d\n", errno);
					return 1;
				}
			}
		}
	}
#ifdef DOWN_TASK_DEBUG
	fclose(fp);
#endif
}
