#include "../inc/down_task.h"
#include <pthread.h>
#include <curl/curl.h>


/*
 * 为了达到能同时创建多个下载任务，采用多线程的方法进行
 * 线程上限目前先约定为50
 * */
int now_down_task_count = 0;	//设计到数据同步访问

int prepare_down_task()
{
	curl_global_init(CURL_GLOBAL_ALL);
	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

void *down_task(void *url)
{
	/*
	 * 若正在下载任务数小于最大并行下载数限制，则直接创建新的下载线程；
	 * 若正在下载数大于限制数，将该任务扔到任务列表中去；
	 * 在每一个任务下载玩之后去任务列表中找出第一下进行下载。
	 * */
	CURL *curl;
	curl = curl_easy_init();

//	char file_name[16], *url_out;
//	strncpy(file_name, url, 16);
//	url_out = (unsigned char *)(&url[16]);

	//还需要用数据库的id对文件进行取名
	char file_name[16]="abc\0";
	FILE *fp = fopen(file_name, "wb");
	if(fp == NULL)
	{
		fprintf(stderr, "fopen error\n");
	}
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	curl_easy_perform(curl);

	fclose(fp);

	curl_easy_cleanup(curl);
	return 0;
}
