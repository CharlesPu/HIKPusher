#ifndef __PUSH_LOG_H__
#define __PUSH_LOG_H__

#include <unistd.h>//close、read、write函数需要

/* error num */
#define ERR_NONE 	0
#define ERR_1		1
#define ERR_2		2

int LOG_Init(void);
int LOG_Save(char* data, int len);
int LOG_Print(unsigned int err_n, const char* format, ...);
int LOG_Free(void);

#endif