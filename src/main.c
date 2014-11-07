/*
 * 这里主要创建三个线程，接收消息，发送消息，创建下载线程
 * */

#include "../inc/down_types.h"
#include "../inc/recv_task.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>


int main()
{
	//消息队列id
	int msgid_recv = -1;
	int msgid_reply = -1;
	//创建队列消息
	printf("初始化消息队列---接受任务消息队列\n");
	msgid_recv = msgget((key_t)IDENTIFIER_RECV, 0666|IPC_CREAT);
	if(msgid_recv == -1)
	{
		fprintf(stderr, "msgget failed with error:%d\n", errno);
		exit(EXIT_FAILURE);
	}
	printf("初始化消息队列完成\n");

	printf("初始化消息队列---回复信息消息队列\n");
	msgid_reply = msgget((key_t)IDENTIFIER_REPLY, 0666|IPC_CREAT);
	if(msgid_reply == -1)
	{
		fprintf(stderr, "msgget failed with error:%d\n", errno);
		exit(EXIT_FAILURE);
	}
	printf("初始化消息队列完成\n");

	//创建监听线程
	int err;
	pthread_t pid_recv;
	msgid_t msgid;
	msgid.msgid_recv = msgid_recv;
	msgid.msgid_reply = msgid_reply;
	err = pthread_create(&pid_recv, NULL, recv_msg, &msgid);
	printf("创建新线程完成\n");

	void *res;
	err = pthread_join(pid_recv, &res);

	return 0;
}
