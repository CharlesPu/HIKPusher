/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __PARSER_H264_H__
#define __PARSER_H264_H__

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

typedef struct _NalUnit  
{  
	int type;  
    int size;  
	unsigned char *data;  
}NalUnit;

int H264_SendToRtmp(struct _ipc* );  
int H264_ParserNALU(NalUnit* nalu, struct _ipc* ipc);
int H264_SendH264Packet(unsigned char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp, struct _ipc*);
int H264_SendVideoSpsPps(RTMP *rtmp, unsigned char *pps, int pps_len, unsigned char * sps, int sps_len, unsigned int nTimeStamp);
int H264_SendPacket(RTMP *rtmp, unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp);

#endif