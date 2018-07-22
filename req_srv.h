/*******************************
@@Author     : Charles
@@Date       : 2017-06-26
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __REQ_LISTEN_SRV_H__
#define __REQ_LISTEN_SRV_H__ 

#include <netinet/in.h>
#include <sys/epoll.h>
#include "config.h"

struct _srv
{
	int listen_fd;
	int clie_fd;
	int connect_num;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clie_addr;
	//初始的接收与发送数组
	unsigned char* rxbuf;
	unsigned char* txbuf;

	int ep_fd;
	// struct epoll_event evt_temp;
	struct epoll_event evts[REQ_SRV_CLIE_MAX_NUM];//传出的各个连接信息
};

int REQ_ServerInit(struct _srv *);
int REQ_SetNonblock(int fd);
int REQ_ServerAccept(struct _srv *);//与客户端的连接
int REQ_ServerSend(int , unsigned char* ,int);
int REQ_ServerRecv(int, struct _srv *);
int REQ_FreeServer(struct _srv *server);


#endif