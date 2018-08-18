/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  
#include "config.h"
#include "ps-rtmp.h"
#include "ps-h264.h"
#include "h264-rtmp.h"
#include "rtmp_sys.h"  
#include "rtmp_log.h" 
#include "log.h" 
/*************************************************
@Description: 从PS流中提取出H264,并发送到rtmp服务器！
@Input: buffer-ps流起始位置   length-流目前长度  
@Output: 
@Return: 
@Others: 
*************************************************/ 
int PS_SendToRtmp(struct _ipc *ipc, char* buffer, int length)
{
    int left_len        = length;  
    char *next_posi     = buffer;   
    char *PayloadData   = NULL;   
    int PayloadDataLen  = 0;

    char *full_h264pack = ipc->full_h264pack;
    //开始循环提取h264并发送！
    // RTMP_LogPrintf("Start to send data ...\n"); 
    while(left_len >= 4)  // >= sizeof(PS_EACH_PACKET_START)
    {           
        if(next_posi   
            && next_posi[0] == '\x00'   
            && next_posi[1] == '\x00'  
            && next_posi[2] == '\x01'  
            && next_posi[3] == '\xBA')  //如果是PS头包
        {  
            if(PS_ProcessHeadPacket(next_posi, left_len, &next_posi, &left_len) == 0)
            {
                break;
            }
        }else 
        if(next_posi   
        && next_posi[0] == '\x00'   
        && next_posi[1] == '\x00'   
        && next_posi[2] == '\x01'   
        && next_posi[3] == '\xE0')  //如果是PES包
        {  
            //接着就是流包，说明是非i帧  
            if(PS_ProcessPESPacket(next_posi, left_len, &next_posi, &left_len, &PayloadData, &PayloadDataLen))  
            {  
                if(PayloadDataLen)  
                {   
                    // for (int i = 0 ; i < 10; ++i)
                    //     printf("%02x ", (unsigned char)PayloadData[i]); 
                    // printf("\n");
                    if (PayloadData[0] == 0x00 && PayloadData[1] == 0x00 && 
                        (PayloadData[2] == 0x01 || (PayloadData[2] == 0x00 && PayloadData[03] == 0x01)))
                    {
                        if(ipc->full_h264pack_len)
                        { 
                            H264_SendToRtmp(full_h264pack, ipc->full_h264pack_len, NULL, ipc); 
                        }
                        //start to store a new h264 pack
                        for (int i = 0; i < H264_FULLPACK_MAX_SIZE; ++i)
                            full_h264pack[i] = 0;
                        ipc->full_h264pack_len = 0;
                        memcpy(full_h264pack, PayloadData, PayloadDataLen);
                        ipc->full_h264pack_len += PayloadDataLen;
                    }else
                    {
                        memcpy(full_h264pack + ipc->full_h264pack_len, PayloadData, PayloadDataLen);
                        ipc->full_h264pack_len += PayloadDataLen;
                    }
                }  
            }  
            else   //说明最后一包H264数据了
            {  
                if(PayloadDataLen)  
                {  
                    // for (int i = 0 ; i < 10; ++i)
                    //     printf("%02x ", (unsigned char)PayloadData[i]); 
                    // printf("\n");
                    if (PayloadData[0] == 0x00 && PayloadData[1] == 0x00 && 
                        (PayloadData[2] == 0x01 || (PayloadData[2] == 0x00 && PayloadData[03] == 0x01)))
                    {   
                        if(ipc->full_h264pack_len)
                        {
                            H264_SendToRtmp(full_h264pack, ipc->full_h264pack_len, NULL, ipc);  
                        }
                        //start to store a new h264 pack
                        for (int i = 0; i < H264_FULLPACK_MAX_SIZE; ++i)
                            full_h264pack[i] = 0;
                        ipc->full_h264pack_len = 0;
                        memcpy(full_h264pack, PayloadData, PayloadDataLen);
                        ipc->full_h264pack_len += PayloadDataLen;
                    }else
                    {
                        memcpy(full_h264pack + ipc->full_h264pack_len, PayloadData, PayloadDataLen);
                        ipc->full_h264pack_len += PayloadDataLen;
                    }
                }  
                break;  
            }  
        }  
        else if(next_posi   
            && next_posi[0] == '\x00'   
            && next_posi[1] == '\x00'  
            && next_posi[2] == '\x01'  
            && next_posi[3] == '\xBC')  //如果是PS map包
        {  
            if(PS_ProcessMapPacket(next_posi, left_len, &next_posi, &left_len, &PayloadData, &PayloadDataLen) == 0)  
            {
                break;
            }
        }  
        else   //如果啥都不是
        {
            // printf("[%d]:0x%02x \n", length - left_len, (unsigned char)*next_posi);
            // printf("leflen:%d, length:%d\n", left_len,length);
            // for (int i = length - left_len ; i < length - 1; ++i)
            //     printf("%02x ", buffer[i]);  
            next_posi++;//往后走
            left_len--;
            // break;
        }  
    }    
    // RTMP_LogPrintf("Send Data Over!\n");  
    return 0; 
}