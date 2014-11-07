#ifndef __DOWN_TASK_H__
#define __DOWN_TASK_H__ 1

#define MSG_MAX_SIZE 3072		//消息最大长度
#define MSG_BACK_MAX_SIZE 12	//回传消息最大长度

#define IDENTIFIER_RECV 12345		//消息队列识别符号
#define IDENTIFIER_REPLY 13579

typedef struct msg_web_cache_t_
{
	long msgtype;
	//消息内容前4个字节是flag，接着是8字节的指针地址，然后是外部url地址
	unsigned char mtext[MSG_MAX_SIZE];
}msg_web_cache_t;

typedef struct msg_back_web_cache_t_
{
	long msgtype;
	unsigned char mtext[MSG_BACK_MAX_SIZE];
}msg_back_web_cache_t;

typedef struct msgid_t_
{
	int msgid_recv;
	int msgid_reply;
}msgid_t;


#endif
