/*******************************
@@Author     : Charles
@@Date       : 2018-06-24
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "parser_h264.h"
#include "rtmp_log.h"  
#include "rtmp_sys.h"   
#include "amf.h"  
#include "sps_dec.h"
#include "log.h"

int H264_Parser(struct _ipc *ipc, const char *pes_payload, int pes_payload_len)
{
    // printf("len:%d\n", pes_payload_len);
    // for (int i = 0; i < 50; ++i)
    //     printf("0x%02x ", (unsigned char)pes_payload[i]);
    // printf("\n");
    printf("len:%d\n", ipc->h264pack_buf_len);
    for (int i = 0; i < 50; ++i)
        printf("0x%02x ", (unsigned char)ipc->h264pack_buf[i]);
    printf("\n");

    int len_tmp = ipc->h264pack_buf_len + pes_payload_len;
    if ( len_tmp > H264PACK_BUF_MAX_SIZE)
    {
        LOG_WARN("H264 buf overflow!");
        return 1;
    }
    char *buf_tmp = ipc->h264pack_buf;
    memcpy(buf_tmp + ipc->h264pack_buf_len, pes_payload, pes_payload_len);//先全部复制到h264缓冲区再处理
    int head = 0, posi = 0;
    for (; posi < len_tmp - 3; ++posi)
    {
        if (buf_tmp[posi] == 0x00
        && buf_tmp[posi + 1] == 0x00
        && (buf_tmp[posi + 2] == 0x01 
            || (buf_tmp[posi + 2] == 0x00
                && buf_tmp[posi + 3] == 0x01)))//界定符
        {
            NalUnit nalUnit; 
            int tmp = H264_ParserNALU(&nalUnit, buf_tmp + head, posi - head);
            if (!tmp)//得到一个nalu就发送
            {
                H264_SendToRtmp(ipc, &nalUnit);
            }//解析失败说明posi之前没有nalu或者是错误的nalu，则直接丢弃
            head = posi;//head指向下一个nalu的开始
        }else
        {
            
        }
    }
    //把剩余的复制到buf中去
    int len_left = len_tmp - head;
    memmove(buf_tmp, buf_tmp + head, len_left);
    memset(buf_tmp + len_left, 0, H264PACK_BUF_MAX_SIZE - len_left);
    ipc->h264pack_buf_len = len_left;//更新buf缓冲大小

    return 0;
}

int H264_ParserNALU(NalUnit* nalu, char *nalu_buf, int nalus_buf_len) 
{
    int nalustart = 0;//nal的开始标识符是几个00
    if (nalus_buf_len < 4) return 1;
    nalu->data = NULL;
#ifdef PRINT_H264_NALU_RD
    printf("read_first_nalu:  ");
    for (int i = 0; i < 10; ++i)
        printf("0x%02x ", nalu_buf[i]);
    printf("\n");
#endif
     //search for nal header   00 00 01 or 00 00 00 01
    if (   nalu_buf[0] == 0x00
        && nalu_buf[1] == 0x00)
    {
        if (nalu_buf[2] == 0x01)
            nalustart = 3;
        else 
        if (   nalu_buf[2] == 0x00
            && nalu_buf[3] == 0x01)
            nalustart = 4;
        else
            return 2;
    }else
        return 3;
    
    nalu->type = nalu_buf[nalustart] & 0x1f; 
    nalu->data = nalu_buf + nalustart;         
    nalu->size = nalus_buf_len - nalustart;

    return 0;
}

int H264_SendToRtmp(struct _ipc* ipc, const NalUnit* nalu)
{    
    switch(nalu->type)
    {
        case NALU_TYPE_SPS:
        {
            ipc->metaData.nSpsLen = nalu->size;
            if (ipc->metaData.Sps == NULL)
                ipc->metaData.Sps = (unsigned char*)malloc(nalu->size);
            memcpy(ipc->metaData.Sps, nalu->data, nalu->size);
            break;
        }
        case NALU_TYPE_PPS:
        {
            ipc->metaData.nPpsLen = nalu->size;
            if (ipc->metaData.Pps == NULL)
                ipc->metaData.Pps = (unsigned char*)malloc(nalu->size);
            memcpy(ipc->metaData.Pps, nalu->data, nalu->size);
            // 解码SPS PPS,获取视频图像宽、高信息   
            int width = 0, height = 0, fps = 0;  
            H264_DecodeSPS(ipc->metaData.Sps, ipc->metaData.nSpsLen, &width, &height, &fps);  
            //ipc->metaData.nWidth = width;  
            //ipc->metaData.nHeight = height;  
            if(fps)
                ipc->metaData.nFrameRate = fps; 
            else
                ipc->metaData.nFrameRate = 25;
            ipc->tick_gap = 1000 / ipc->metaData.nFrameRate;
            break;
        }
        default:
        {
            int bKeyframe  = (nalu->type == NALU_TYPE_IDR) ? TRUE : FALSE; // key frame
            H264_SendH264Packet(nalu->data, nalu->size, bKeyframe, ipc->tick, ipc);
#ifdef PRINT_RTMP_SEND_SIZE
            LOG_INFO("%s NALU size:%8d\n", ipc->dev_id, nalu->size);
#endif
            ipc->tick += ipc->tick_gap; 
            msleep(RTMP_SEND_INTVL);
            break;
        }
    }

    return 0;  
} 

/*************************************************
@Description: 发送H264数据帧
@Input:  data 存储数据帧内容  size 数据帧的大小  bIsKeyFrame 记录该帧是否为关键帧   nTimeStamp 当前帧的时间戳
@Output: 
@Return: 0 - success, others - fail 
@Others: 
*************************************************/ 
int H264_SendH264Packet(char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp, struct _ipc* ipc)  
{ 
    if(data == NULL && size < 11){  
        return -1;  
    }  
    char *body = (char*)malloc(size + 9); 
    memset(body, 0, size + 9);
    int i = 0; 
    if(bIsKeyFrame){  
        body[i++] = 0x17;// 1:Iframe  7:AVC   
        body[i++] = 0x01;// AVC NALU_TYPE_AUDU   
        body[i++] = 0x00;  
        body[i++] = 0x00;  
        body[i++] = 0x00;  
        // NALU size   
        body[i++] = size>>24 & 0xff;  
        body[i++] = size>>16 & 0xff;  
        body[i++] = size>>8  & 0xff;  
        body[i++] = size     & 0xff;
        // NALU data   
        memcpy(&(body[i]), data, size);  
        H264_SendVideoSpsPps(ipc->rtmp, ipc->metaData.Pps, ipc->metaData.nPpsLen, ipc->metaData.Sps, ipc->metaData.nSpsLen, nTimeStamp);
        msleep(RTMP_SEND_INTVL);
    }else{  
        body[i++] = 0x27;// 2:Pframe  7:AVC   
        body[i++] = 0x01;// AVC NALU   
        body[i++] = 0x00;  
        body[i++] = 0x00;  
        body[i++] = 0x00;  
        // NALU size   
        body[i++] = size>>24 & 0xff;  
        body[i++] = size>>16 & 0xff;  
        body[i++] = size>>8  & 0xff;  
        body[i++] = size     & 0xff;
        // NALU data   
        memcpy(&(body[i]), data, size);  
    }  

    int bRet = H264_SendPacket(ipc->rtmp, RTMP_PACKET_TYPE_VIDEO, body, i + size, nTimeStamp);  
    if (body != NULL)
    {
        free(body);
        body = NULL;
    }
    
    return bRet;  
} 
/*************************************************
@Description: 发送视频的sps和pps信息
@Input: pps 存储视频的pps信息  pps_len 视频的pps信息长度  sps 存储视频的pps信息  sps_len 视频的sps信息长度
@Output: 
@Return: 0 - fail 
@Others: 
*************************************************/ 
int H264_SendVideoSpsPps(RTMP *rtmp, unsigned char *pps, int pps_len, unsigned char * sps, int sps_len, unsigned int nTimestamp)
{
    RTMPPacket * packet  = NULL;//rtmp包结构
    unsigned char * body = NULL;

    packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, 32766);  
    RTMPPacket_Reset(packet); 

    body = (unsigned char *)packet->m_body;
    int i = 0;
    body[i++] = 0x17;
    body[i++] = 0x00;

    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    /*AVCDecoderConfigurationRecord*/
    body[i++] = 0x01;
    body[i++] = sps[1];
    body[i++] = sps[2];
    body[i++] = sps[3];
    body[i++] = 0xff;

    /*sps*/
    body[i++] = 0xe1;
    body[i++] = (sps_len >> 8) & 0xff;
    body[i++] = sps_len & 0xff;
    memcpy(&(body[i]), sps, sps_len);
    i += sps_len;

    /*pps*/
    body[i++] = 0x01;
    body[i++] = (pps_len >> 8) & 0xff;
    body[i++] = (pps_len) & 0xff;
    memcpy(&(body[i]), pps, pps_len);
    i +=  pps_len;

    packet->m_packetType        = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize         = i;
    packet->m_nChannel          = 0x04;
    packet->m_nTimeStamp        = nTimestamp;
    packet->m_hasAbsTimestamp   = 0;
    packet->m_headerType        = RTMP_PACKET_SIZE_MEDIUM;
    packet->m_nInfoField2       = rtmp->m_stream_id;
    /*调用发送接口*/
    int nRet = RTMP_SendPacket(rtmp, packet, TRUE);
    if (!nRet)
    {  
        LOG_ERROR(ERR_RTMP_SEND, "RTMP Send Error\n");  
        // RTMP_Log(RTMP_LOGERROR,"Send Error\n");  
    }
    if (packet != NULL)
    {  
        RTMPPacket_Free(packet);      
        free(packet);  
        packet = NULL;  
    }

    return nRet;
}
/*************************************************
@Description: 发送RTMP数据包
@Input: nPacketType 数据类型  data 存储数据内容  size 数据大小  nTimestamp 当前包的时间戳
@Output: 
@Return: 成功则返回 1 , 失败则返回一个小于0的数
@Others: 
*************************************************/ 
int H264_SendPacket(RTMP *rtmp, unsigned int nPacketType,char *data,unsigned int size,unsigned int nTimestamp)  
{  
    RTMPPacket* packet;
    /*分配包内存和初始化,len为包体长度*/
    packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, size);  //actully alloc "body"
    RTMPPacket_Reset(packet);  
    /*包体内存*/
    memcpy(packet->m_body, data, size);
    packet->m_nBodySize         = size;
    packet->m_hasAbsTimestamp   = 0;
    packet->m_packetType        = nPacketType; /*此处为类型有两种一种是音频,一种是视频*/
    packet->m_nInfoField2       = rtmp->m_stream_id;
    packet->m_nChannel          = 0x04;
    packet->m_headerType        = RTMP_PACKET_SIZE_LARGE;
    packet->m_nTimeStamp        = nTimestamp;
    if (RTMP_PACKET_TYPE_AUDIO == nPacketType && size !=4)
    {
        packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    }
    /*发送*/
    int nRet =0;
    if (RTMP_IsConnected(rtmp))
    {
        nRet = RTMP_SendPacket(rtmp, packet, TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
    }
    /*释放内存, carefull that RTMPPacket_Free-free body, free-free packet mem*/
    if (packet != NULL)
    {  
        RTMPPacket_Free(packet);      
        free(packet); 
        packet = NULL;  
    }

    return nRet;  
}  
