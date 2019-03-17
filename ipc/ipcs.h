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
    char *pespack_buf;				//PES解析器缓冲区，为一个PES包荷载数据
    int pespack_buf_len;			//当前PES解析器缓冲区中一个PES包荷载数据长度，可能不完整
    int pespack_left_len;			//下一次需要继续解析的残余长度
    char *h264pack_buf;				//H264解析器缓冲区，实际为n个NALU
    int h264pack_buf_len;			//当前H264解析器缓冲区中n个NALU长度，可能不完整
    /*封装RTMP流相关*/
    RTMP *rtmp;
	RTMPMetadata  metaData;			//元数据
	unsigned int  tick;
	unsigned int  tick_gap;
};

int IPCS_Init(void);
int IPCS_PushInit(struct _ipc *);
int IPCS_PushFree(struct _ipc *);
unsigned short IPCS_GetInt_Devid(unsigned char *msgs, int len);



#endif