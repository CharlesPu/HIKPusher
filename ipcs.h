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
#include "log.h"  
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
	long login_id;
	long stream_handle;
	long preview_session_id; //struPreviewOut.lSessionID
	char dev_id[13];
	struct timeval last_req_time;
	char push_state;//is pushing or not 
	char online_state; 
	/*ps send*/
	RTMP *rtmp;
	char *full_h264pack;
    int full_h264pack_len;
    /*h264 send*/
    unsigned char *m_pFileBuf;  
	unsigned int  m_nFileBufSize; 
	unsigned int  nalhead_pos;
	RTMPMetadata metaData;
	unsigned int tick;
	unsigned int tick_gap; 
	int pps_sps_flag;
};

int IPCS_Init(void);
int IPCS_PushInit(struct _ipc *);
int IPCS_PushFree(struct _ipc *);
short IPCS_GetInt_Devid(unsigned char *msgs, int len);



#endif