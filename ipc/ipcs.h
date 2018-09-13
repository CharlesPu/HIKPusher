/*******************************
@@Author     : Charles
@@Date       : 2018-07-04
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __IPC_H__
#define __IPC_H__

#include <sys/time.h>
#include "rtmp_sys.h"  
/*** _RTMPMetadata 内部结构体。该结构体主要用于存储和传递元数据信息*/ 
typedef struct _RTMPMetadata  
{  
	// video, must be h264 type   
	unsigned int    nWidth;  
	unsigned int    nHeight;  
	unsigned int    nFrameRate; //帧率
	unsigned int    nSpsLen;  
	unsigned char   *Sps;  
	unsigned int    nPpsLen;  
	unsigned char   *Pps;   
} RTMPMetadata;  

struct _ipc
{
	/*交互状态相关*/
	long login_id;					//系统分配的登录ID
	long stream_handle;				//实时流句柄
	long preview_session_id; 		//系统分配的预览会话ID                         struPreviewOut.lSessionID
	char dev_id[13];				//字符串形式的设备ID
	struct timeval last_req_time;	//上一次预览请求时间
	char push_state;				//是否在推流
	char online_state;				//是否在线
	/*解析RTP流相关*/
	char *full_h264pack;
    int  full_h264pack_len;
    /*封装RTMP流相关*/
    RTMP *rtmp;
    unsigned char *m_pFileBuf;  
	unsigned int  m_nFileBufSize; 
	unsigned int  nalhead_pos;
	RTMPMetadata  metaData;
	unsigned int  tick;
	unsigned int  tick_gap; 
	int 	      pps_sps_flag;
};

int IPCS_Init(void);
int IPCS_PushInit(struct _ipc *);
int IPCS_PushFree(struct _ipc *);
unsigned short IPCS_GetInt_Devid(unsigned char *msgs, int len);



#endif