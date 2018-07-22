/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __H264_RTMP_H__
#define __H264_RTMP_H__

#include "rtmp_sys.h"  
#include "ipcs.h"

#define NALU_TYPE_SLICE 	0x01
#define NALU_TYPE_DPA 		0x02
#define NALU_TYPE_DPB 		0x03
#define NALU_TYPE_DPC 		0x04
#define NALU_TYPE_IDR 		0x05
#define NALU_TYPE_SEI 		0x06          
#define NALU_TYPE_SPS 		0x07
#define NALU_TYPE_PPS 		0x08
#define NALU_TYPE_AUD 		0x09
#define NALU_TYPE_EOSEQ 	0x0a
#define NALU_TYPE_EOSTREAM 	0x0b
#define NALU_TYPE_FILL 		0x0c

//定义包头长度，RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket) + RTMP_MAX_HEADER_SIZE)
//存储Nal单元数据的buffer大小
#define BUFFER_SIZE 32768
//搜寻Nal单元时的一些标志
#define GOT_A_NAL_CROSS_BUFFER (BUFFER_SIZE + 1)
#define GOT_A_NAL_INCLUDE_A_BUFFER (BUFFER_SIZE + 2)
#define NO_MORE_BUFFER_TO_READ (BUFFER_SIZE + 3)
/*** _NaluUnit 内部结构体。该结构体主要用于存储和传递Nal单元的类型、大小和数据*/ 
typedef struct _NaluUnit  
{  
	int type;  
    int size;  
	unsigned char *data;  
}NaluUnit;

// /*** _RTMPMetadata 内部结构体。该结构体主要用于存储和传递元数据信息*/ 
// typedef struct _RTMPMetadata  
// {  
// 	// video, must be h264 type   
// 	unsigned int    nWidth;  
// 	unsigned int    nHeight;  
// 	unsigned int    nFrameRate; //帧率
// 	unsigned int    nSpsLen;  
// 	unsigned char   *Sps;  
// 	unsigned int    nPpsLen;  
// 	unsigned char   *Pps;   
// } RTMPMetadata,*LPRTMPMetadata;  

enum  
{  
	 VIDEO_CODECID_H264 = 7,  
};     

int H264_SendToRtmp(RTMP *rtmp, char* h264_buffer, int length, int (*read_buffer)(unsigned char *buf, int buf_size), struct _ipc* );  
int H264_ReadOneNalu(NaluUnit* nalu, int (*read_buffer)(uint8_t *buf, int buf_size), struct _ipc *); 
int H264_ReadFirstNalu(NaluUnit* nalu, struct _ipc *);
int H264_SendH264Packet(RTMP *rtmp, unsigned char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp, struct _ipc*);
int H264_SendVideoSpsPps(RTMP *rtmp, unsigned char *pps, int pps_len, unsigned char * sps, int sps_len, unsigned int nTimeStamp);
int H264_SendPacket(RTMP *rtmp, unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp);
int H264_FreeSpsPps(void);


int SendH264ToRtmp(RTMP *rtmp, char* h264_buf, int h264_len);

#endif