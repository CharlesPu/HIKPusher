/*******************************
@@Author     : Charles
@@Date       : 2017-06-24
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "h264-rtmp.h"
#include "rtmp_log.h"  
#include "rtmp_sys.h"   
#include "amf.h"  
#include "sps_dec.h"
#include "log.h"

static int res = 0;
/**
 * 将内存中的一段H.264编码的视频数据利用RTMP协议发送到服务器
 *
 * @param read_buffer 回调函数，当数据不足的时候，系统会自动调用该函数获取输入数据。
 *                  2个参数功能：
 *                  uint8_t *buf：外部数据送至该地址
 *                  int buf_size：外部数据大小
 *                  返回值：成功读取的内存大小
 * @成功则返回0 , 失败则返回非0
 */ 
int H264_SendToRtmp(char* h264_buffer, int h264_length, int (*read_buffer)(unsigned char *buf, int buf_size), struct _ipc* ipc)
{    
    // uint32_t now,last_update;
    ipc->nalhead_pos = 0;
    ipc->m_pFileBuf = (unsigned char*)h264_buffer;
    ipc->m_nFileBufSize = h264_length;
    int *flag = &(ipc->pps_sps_flag);
    // printf("buflen%d\n", m_nFileBufSize);
    // for (int i = 0 ; i < 10; ++i)
    //     printf("%02x ", (unsigned char)m_pFileBuf[i]); 
    // printf("\n");
    NaluUnit naluUnit;  
    // 读取SPS帧   
    switch ((*flag))
    {
        case 0:
            {
                memset(&(ipc->metaData), 0, sizeof(RTMPMetadata));
                H264_ReadFirstNalu(&naluUnit, ipc);  
                ipc->metaData.nSpsLen = naluUnit.size;  
                ipc->metaData.Sps     = NULL;
                ipc->metaData.Sps     = (unsigned char*)malloc(naluUnit.size);
                memcpy(ipc->metaData.Sps, naluUnit.data, naluUnit.size);

                (*flag)++;
                break;
            }
        case 1:
            {    // 读取PPS帧   
                H264_ReadOneNalu(&naluUnit, read_buffer, ipc);  
                ipc->metaData.nPpsLen = naluUnit.size; 
                ipc->metaData.Pps     = NULL;
                ipc->metaData.Pps     = (unsigned char*)malloc(naluUnit.size);
                memcpy(ipc->metaData.Pps, naluUnit.data, naluUnit.size);
                // 解码SPS PPS,获取视频图像宽、高信息   
                int width = 0, height = 0, fps = 0;  
                H264_DecodeSPS(ipc->metaData.Sps, ipc->metaData.nSpsLen, &width, &height, &fps);  
                //ipc->metaData.nWidth = width;  
                //ipc->metaData.nHeight = height;  
                // printf("fps:%d\n", fps);
                if(fps)
                    ipc->metaData.nFrameRate = fps; 
                else
                    ipc->metaData.nFrameRate = 25;
                ipc->tick_gap = 1000 / ipc->metaData.nFrameRate;
                //发送PPS,SPS
                //SendVideoSpsPps(ipc->metaData.Pps,ipc->metaData.nPpsLen,ipc->metaData.Sps,ipc->metaData.nSpsLen);
                (*flag)++;
                break;
            }
        default:
            {
                while(!(res=H264_ReadOneNalu(&naluUnit, read_buffer, ipc)))  
                {    
                    if (naluUnit.type != NALU_TYPE_SPS && naluUnit.type != NALU_TYPE_PPS)
                    {
                        int bKeyframe  = (naluUnit.type == NALU_TYPE_IDR) ? TRUE : FALSE; // 5-IDR，首个I帧
                        H264_SendH264Packet(naluUnit.data, naluUnit.size, bKeyframe, ipc->tick, ipc);
#ifdef PRINT_RTMP_SEND_SIZE
                        LOG_Print(ERR_NONE, "%s NALU size:%8d\n", ipc->dev_id, naluUnit.size);
#endif
                        ipc->tick += ipc->tick_gap;
                        // now   = RTMP_GetTime();
                        // msleep(tick_gap-now+last_update);  
                        msleep(RTMP_SEND_INTVL);
                    }
                } 
                break;
            }
    }

    return 0;  
}  
/**
 * 从内存中读取出第一个Nal单元
 *
 * @param nalu 存储nalu数据
 * @param read_buffer 回调函数，当数据不足的时候，系统会自动调用该函数获取输入数据。
 *                  2个参数功能：
 *                  uint8_t *buf：外部数据送至该地址
 *                  int buf_size：外部数据大小
 *                  返回值：成功读取的内存大小
 * @成功则返回 0 , 失败则返回非0
 */
int H264_ReadFirstNalu(NaluUnit* nalu, struct _ipc* ipc) 
{
    unsigned int naltail_pos = 0;
    nalu->data = NULL;
#ifdef PRINT_H264_NALU_RD
    printf("read_first_nalu:  ");
    for (int i = ipc->nalhead_pos; i < 10; ++i)
        printf("0x%02x ", ipc->m_pFileBuf[i]);
    printf("\n");
#endif
    while(naltail_pos < ipc->m_nFileBufSize)  
    {  
        //search for nal header   00 00 01 或者 00 00 00 01
        if(ipc->m_pFileBuf[naltail_pos++] == 0x00 && 
            ipc->m_pFileBuf[naltail_pos++] == 0x00) 
        {
            if(ipc->m_pFileBuf[naltail_pos++] == 0x01)
            {
                ipc->nalhead_pos = naltail_pos;
                nalu->size  = ipc->m_nFileBufSize - 3;
            }
            else 
            { //cuz we have done an i++ before,so we need to roll back now
                naltail_pos--;      
                if(ipc->m_pFileBuf[naltail_pos++] == 0x00 && 
                    ipc->m_pFileBuf[naltail_pos++] == 0x01)
                {
                    ipc->nalhead_pos = naltail_pos;
                    nalu->size = ipc->m_nFileBufSize - 4;
                }
                else
                    continue;
            }
        }
        else 
            continue;
        
        nalu->type   = ipc->m_pFileBuf[naltail_pos] & 0x1f; 
        nalu->data = ipc->m_pFileBuf + ipc->nalhead_pos;

        return 0;           
    }

    return 1;
}

/**
 * 从内存中读取出一个Nal单元
 *
 * @param nalu 存储nalu数据
 * @param read_buffer 回调函数，当数据不足的时候，系统会自动调用该函数获取输入数据。
 *                  2个参数功能：
 *                  uint8_t *buf：外部数据送至该地址
 *                  int buf_size：外部数据大小
 *                  返回值：成功读取的内存大小
 * @成功则返回 0 , 失败则返回非0
 */
int H264_ReadOneNalu(NaluUnit* nalu, int (*read_buffer)(uint8_t *buf, int buf_size), struct _ipc* ipc)  
{    
    unsigned int naltail_pos = ipc->nalhead_pos;//printf("hea:%d\n", ipc->nalhead_pos);
    unsigned int *head_pos    = &(ipc->nalhead_pos);
    int nalustart = 0;//nal的开始标识符是几个00
    unsigned int file_buf_size = ipc->m_nFileBufSize;
    unsigned char *file_buf = ipc->m_pFileBuf;
    // unsigned char* m_pFileBuf_tmp_old = NULL;  //used for realloc
    nalu->data = NULL;

    //此处NO_MORE_BUFFER_TO_READ = BUFFER_SIZE + 3， 3是因为下面在最后进行00 00 01(或者00) 判断时加了3次
    //此处不用担心越界，因为不是指针，是字节数组的下标
    if((*head_pos) >= file_buf_size)
    {
        return 1;
    }
#ifdef PRINT_H264_NALU_RD
    printf("read_one_nalu:  ");
    for (int i = (*head_pos); i < 10; ++i)
        printf("0x%02x ", file_buf[i]);
    printf("\n");
#endif
    while(naltail_pos < file_buf_size)  
    {  
        //search for nal tail
        if(file_buf[naltail_pos++] == 0x00 && 
            file_buf[naltail_pos++] == 0x00) 
        {
            if(file_buf[naltail_pos++] == 0x01)
            {   
                nalustart = 3;
            }
            else 
            {
                //cuz we have done an i++ before,so we need to roll back now
                naltail_pos--;      
                if(file_buf[naltail_pos++] == 0x00 && 
                    file_buf[naltail_pos++] == 0x01)
                {
                    nalustart = 4;
                }
                else
                    continue;
            }
        }
        else 
            continue;
        if ((*head_pos) == 0)
        {
            (*head_pos) = naltail_pos;
            continue;
        }
        // middle ones
        nalu->type = file_buf[(*head_pos)] & 0x1f; 
        nalu->size = naltail_pos - (*head_pos) - nalustart;
        if(nalu->type == NALU_TYPE_SEI || nalu->type == NALU_TYPE_AUD)//如果是补充增强信息 (SEI) or 分界符数据
        {
            (*head_pos) = naltail_pos;
            continue;
        }
        // nalu->data = (unsigned char*)malloc(nalu->size);
        // memset(nalu->data, 0, nalu->size);
        // memcpy(nalu->data, file_buf + (*head_pos), nalu->size);
        nalu->data = file_buf + (*head_pos);
        (*head_pos) = naltail_pos;
        return 0;                   
    }

    if (naltail_pos >= file_buf_size)//not find more 00 00 00 01 or 00 00 01, the last one
    {
        // printf("tail:%d, head:%d \n", naltail_pos, (*head_pos));
        nalu->type = file_buf[(*head_pos)] & 0x1f; 
        nalu->size = file_buf_size - (*head_pos);
        if(nalu->type == NALU_TYPE_SEI || nalu->type == NALU_TYPE_AUD)//如果是补充增强信息 (SEI) or 分界符数据
        {
            (*head_pos) = naltail_pos;
            return 7;
        }
        // nalu->data = (unsigned char*)malloc(nalu->size);
        // memset(nalu->data, 0, nalu->size);
        // memcpy(nalu->data, file_buf + (*head_pos), nalu->size);
        nalu->data = file_buf + (*head_pos);
        (*head_pos) = naltail_pos;
        return 0; 
    }

    return 6;  
} 

/**
 * 发送H264数据帧
 *
 * @param data 存储数据帧内容
 * @param size 数据帧的大小
 * @param bIsKeyFrame 记录该帧是否为关键帧
 * @param nTimeStamp 当前帧的时间戳
 *
 * @成功则返回 0, 失败则返回-1
 */
int H264_SendH264Packet(unsigned char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp, struct _ipc* ipc)  
{ 
    if(data == NULL && size < 11){  
        return -1;  
    }  
    unsigned char *body = (unsigned char*)malloc(size + 9); 
    memset(body, 0, size + 9);
    int i = 0; 
    if(bIsKeyFrame){  
        body[i++] = 0x17;// 1:Iframe  7:AVC   
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

    int bRet = 0;
    bRet = H264_SendPacket(ipc->rtmp, RTMP_PACKET_TYPE_VIDEO, body, i + size, nTimeStamp);  
    if (body != NULL)
    {
        free(body);
        body = NULL;
    }
    
    return bRet;  
} 
/**
 * 发送视频的sps和pps信息
 *
 * @param pps 存储视频的pps信息
 * @param pps_len 视频的pps信息长度
 * @param sps 存储视频的pps信息
 * @param sps_len 视频的sps信息长度
 *
 * @成功则返回 1 , 失败则返回0
 */
int H264_SendVideoSpsPps(RTMP *rtmp, unsigned char *pps, int pps_len, unsigned char * sps, int sps_len, unsigned int nTimestamp)
{
    RTMPPacket * packet  = NULL;//rtmp包结构
    unsigned char * body = NULL;
    int i;
    // packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 1024);
    // RTMPPacket_Reset(packet);//重置packet状态
    // // memset(packet, 0, RTMP_HEAD_SIZE + 1024);
    // packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, 32766);  
    RTMPPacket_Reset(packet); 

    body = (unsigned char *)packet->m_body;
    i = 0;
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
        LOG_Print(ERR_NONE, "RTMP Send Error\n");  
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
/**
 * 发送RTMP数据包
 *
 * @param nPacketType 数据类型
 * @param data 存储数据内容
 * @param size 数据大小
 * @param nTimestamp 当前包的时间戳
 *
 * @成功则返回 1 , 失败则返回一个小于0的数
 */
int H264_SendPacket(RTMP *rtmp, unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp)  
{  
    RTMPPacket* packet;
    /*分配包内存和初始化,len为包体长度*/
    packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, size);  //actully alloc "body"
    RTMPPacket_Reset(packet);  
    // memset(packet, 0, RTMP_HEAD_SIZE + size);
    /*包体内存*/
    // packet->m_body              = (char *)packet + RTMP_HEAD_SIZE;
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
