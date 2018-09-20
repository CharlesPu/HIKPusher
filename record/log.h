#ifndef __PUSH_LOG_H__
#define __PUSH_LOG_H__

/* error num */
#define ERR_NONE 					0
#define ERR_VTDU_START				1
#define ERR_CMS_START				2
#define ERR_EPOLL_WAIT				3
#define ERR_EPOLL_CTL_DEL			4
#define ERR_IPC_PUSH_INIT			5
#define ERR_CMS_STREAM_START		6
#define ERR_CMS_STREAM_PUSH			7
#define ERR_CMS_STREAM_STOP			8
#define ERR_VTDU_STOP 				9
#define ERR_VTDU_SETCB				10
#define ERR_RTMP_SET_URL			11
#define	ERR_RTMP_CONNECT  			12
#define ERR_RTMP_CONNECT_STREAM		13
#define ERR_REQ_FD_GET				14
#define ERR_REQ_PORT_BIND			15
#define ERR_REQ_LISTEN				16
#define ERR_REQ_EPOLL_GET			17
#define ERR_REQ_EPOLL_CTL			18
#define ERR_REQ_ACCEPT				19
#define	ERR_REQ_SEND				20
#define ERR_RTMP_SEND				21
#define ERR_CMS_STOP				21
#define ERR_RTMP_SEND				21

#define LOG_INFO(fmt, ...)	\
		LOG_Print("INFO", 0, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)	\
		LOG_Print("WARN", 0, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(n, fmt, ...)	\
		LOG_Print("ERROR", n, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)	\
		LOG_Print("DEBUG", 0, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

int LOG_Init(void);
int LOG_Save(char* data, int len);
int LOG_Print(const char *log_type, int err_n, int line_n, const char *func_name, const char* format, ...);
int LOG_Free(void);

#endif