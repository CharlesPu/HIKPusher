/*******************************
@@Author     :  Charles
@@Date       :  2017-06-26
@@Mail       :  pu17rui@sina.com
@@Description: request listen server
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//close、read、write函数需要
#include <errno.h>
#include <fcntl.h>//F_GETFL设置非阻塞时需要
#include <sys/socket.h>
#include <sys/types.h>//数据类型定义
#include <arpa/inet.h>//ip地址转换
#include "req_srv.h"
#include "log.h"

/*************************************************
@Description: request listen server init
@Input: serv
@Output: 
@Return: 0-success   others-fail
@Others: 
*************************************************/
int REQ_ServerInit(struct _srv *server)
{
	server->connect_num=0;
	server->rxbuf = (unsigned char*)malloc(REQ_SRV_BUF_LEN);
	memset(server->rxbuf, 0, REQ_SRV_BUF_LEN);
	server->txbuf = (unsigned char*)malloc(REQ_SRV_BUF_LEN);
	memset(server->txbuf, 0, REQ_SRV_BUF_LEN);

	server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
	if (server->listen_fd == -1)
	{
		LOG_ERROR(ERR_REQ_FD_GET, "fail to creat a socket!");
		// perror("error:fail to creat a socket!");
		return 1;
	}
	int opt = 1;
	setsockopt(server->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//端口复用
	
	memset(&(server->serv_addr), 0, sizeof(server->serv_addr));
	server->serv_addr.sin_family 		= AF_INET;
//	inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr.s_addr);//直接绑定固定ip而不用本地其他ip
	server->serv_addr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	server->serv_addr.sin_port 			= htons(REQ_SRV_PORT);

	int temp = 0;
	temp = bind(server->listen_fd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr));/*绑定服务器地址结构体*/
	if (temp == -1)
	{
		close(server->listen_fd);
		LOG_ERROR(ERR_REQ_PORT_BIND, "fail to bind port!");
		// perror("error:fail to bind port!");
		return 2;
	}
	temp = listen(server->listen_fd, REQ_SRV_CLIE_MAX_NUM);
	if (temp == -1)
	{
		close(server->listen_fd);
		LOG_ERROR(ERR_REQ_LISTEN, "fail to listen!");
		// perror("error:fail to listen!");
		return 3;
	}
	/************************epoll 创建********************/
	server->ep_fd = epoll_create(REQ_SRV_CLIE_MAX_NUM);
	if (server->ep_fd == -1)
	{
		LOG_ERROR(ERR_REQ_EPOLL_GET, "epoll create error");
		// perror("epoll create error");
		return 4;
	}
	// if (Server::SetNonblock(server->listen_fd))//不知道需不需要！！因为去掉也可！但是注意成功地返回值是0！！！！记住！！
	// 	return 5;
	/*将listen_fd及对应的结构体设置到树上,使ep_fd可找到*/
	struct epoll_event evt_temp;
	evt_temp.data.fd = server->listen_fd;
	evt_temp.events  = EPOLLIN ;
	if(epoll_ctl(server->ep_fd, EPOLL_CTL_ADD, server->listen_fd, &evt_temp) == -1)//挂到红黑树上去
	{
		LOG_ERROR(ERR_REQ_EPOLL_CTL, "epoll_ctl listen_fd fail1");
		// perror("epoll_ctl listen_fd fail1");
		return 6;
	}
	
	return 0;
}

/*************************************************
@Description: set nonblock
@Input: socket
@Output: 
@Return: 0-success   others-fail
@Others: 
*************************************************/
int REQ_SetNonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
	{
		perror("fcntl fail");
		return 1;
	}
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) < 0)
	{
		perror("fcntl fail");
		return 2;
	}

	return 0;	
}

/*************************************************
@Description: accept
@Input: serv
@Output: 
@Return: 0-success   others-fail
@Others: 
*************************************************/
int REQ_ServerAccept(struct _srv *server)
{
	socklen_t clie_addr_len = sizeof(server->clie_addr);//传入传出参数，没啥用感觉
	server->clie_fd = accept(server->listen_fd, (struct sockaddr *)&(server->clie_addr), &clie_addr_len);//建立与客户端的连接

	if (server->clie_fd == -1)
	{
		LOG_ERROR(ERR_REQ_ACCEPT, "accept fail\n");
		// perror("accept fail\n");
		return 1;
	}
	else
	{
		struct epoll_event evt_temp;
		evt_temp.events = EPOLLIN ;
		evt_temp.data.fd= server->clie_fd;
		if(epoll_ctl(server->ep_fd, EPOLL_CTL_ADD, server->clie_fd, &evt_temp) == -1)//然后挂到红黑树上去
		{
			LOG_ERROR(ERR_REQ_EPOLL_CTL, "epoll_ctl clie_fd fail!");
			// perror("epoll_ctl clie_fd fail!");
			return 2;
		}
#ifdef PRINT_REQ_LINK
		// printf("OK: Server has got connection from %s.\n",inet_ntoa(server->clie_addr.sin_addr));
		LOG_INFO("REQ_Server has connected to %s:%d, clie_fd:%d, connect num:%d\n",
            inet_ntoa(server->clie_addr.sin_addr), ntohs(server->clie_addr.sin_port), server->clie_fd, ++(server->connect_num));//打印连接的客户端的编号
#endif
	}	
	return 0;	
}

/*************************************************
@Description: send
@Input: fd, buf, len
@Output: 
@Return: others - send length   -1 - fail
@Others: 
*************************************************/
int REQ_ServerSend(int fd, unsigned char* buf, int length)
{
	int num = write(fd, buf, length);
	if (num != length)
	{
		LOG_ERROR(ERR_REQ_SEND, "send fail!");
		// perror("send fail!");
		return -1;
	}
	return num;
}

/*************************************************
@Description: recv
@Input: serv
@Output: 
@Return: others - recv length   -1 - fail
@Others: 
*************************************************/
int REQ_ServerRecv(int fd, struct _srv *server)
{	
	memset(server->rxbuf, 0, REQ_SRV_BUF_LEN);
	int num = read(fd, server->rxbuf, REQ_SRV_BUF_LEN);
	if (num < 0)
	{
		// perror("read fail!");
		return -1;
	}else 
	if(num > 0)
	{
#ifdef PRINT_REQ_RECV_RAW
		printf("recv length: %d data:\n", num);//将从客户端读到的数据，在屏幕上输出
		for (int i = 0; i < num; i++)
			printf("[%d]%c ", i, server->rxbuf[i]);
		printf("\n");
#endif
	}
	return num;
}

/*************************************************
@Description: free bufs
@Input: serv
@Output: 
@Return: 0-success   others-fail
@Others: 
*************************************************/
int REQ_FreeServer(struct _srv *server)
{
	if(server->rxbuf != NULL)
	{
		free(server->rxbuf);
		server->rxbuf = NULL;
	}
	if(server->txbuf != NULL)
	{
		free(server->txbuf);
		server->txbuf = NULL;
	}

	if(server->listen_fd)
	{
		close(server->listen_fd);
		server->listen_fd = 0;
	}
	if(server->clie_fd)
	{
		close(server->clie_fd);
		server->clie_fd = 0;
	}

	return 0;
}
