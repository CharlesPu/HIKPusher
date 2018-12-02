/*******************************
@@Author     : Charles
@@Date       : 2018-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  
#include "config.h"
#include "rtp-rtmp.h"
#include "parser_ps.h"
#include "parser_h264.h"
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
int RTP_SendToRtmp(struct _ipc *ipc, char* buffer, int length)
{
    int left_len        = length;  
    char *next_posi     = buffer;   
    char *PayloadData   = NULL;   
    int PayloadDataLen  = 0;

    unsigned char *full_h264pack = ipc->full_h264pack;
    
    while(left_len >= 4)  // >= sizeof(PS_EACH_PACKET_START)
    {           
        if(next_posi   
            && next_posi[0] == '\x00'   
            && next_posi[1] == '\x00'  
            && next_posi[2] == '\x01'  
            && next_posi[3] == '\xBA')  //如果是PS头包
        {  
            if(PS_ParserHeadPacket(&next_posi, &left_len))//fail
            {
                break;
            }
        }else 
        if(next_posi   
        && next_posi[0] == '\x00'   
        && next_posi[1] == '\x00'   
        && next_posi[2] == '\x01'   
        && next_posi[3] == '\xE0')  //如果是PES包
        {  //接着就是流包
            if(!PS_ParserPESPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen))  
            {  
                if(PayloadDataLen >= 3) //防止PayloadData[]寻址出错 
                {   
                    // for (int i = 0 ; i < 10; ++i)
                    //     printf("%02x ", (unsigned char)PayloadData[i]); 
                    // printf("\n");
                    if (PayloadData[0] == 0x00 && PayloadData[1] == 0x00 && 
                        (PayloadData[2] == 0x01 || (PayloadData[2] == 0x00 && PayloadData[03] == 0x01)))
                    {
                        if(ipc->full_h264pack_len)//send previous data
                        { 
                            H264_SendToRtmp(ipc); 
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
            else//fail
                break; 
        }  
        else if(next_posi   
            && next_posi[0] == '\x00'   
            && next_posi[1] == '\x00'  
            && next_posi[2] == '\x01'  
            && next_posi[3] == '\xBC')  //如果是PS map包
        {  
            if(PS_ParserMapPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen))//fail
            {
                break;
            }
        }  
        else   //如果啥都不是
        { 
            next_posi++;//往后走
            left_len--;
        }  
    }    
 
    return 0; 
}
/*************************************************
@Description: 从PS流中提取出H264
@Input: buffer-流起始位置   length-流目前长度    h264_buf-H264需要存储的位置    h264_len-H264的长度
@Output: 
@Return: 读取到的H264长度
@Others: 
*************************************************/ 
int RTP_GetH264(char* buffer, int length, char *h264_buf, int *h264_len)  
{  
    int left_len    = length;  
    char *next_posi = buffer;  

    // if(PS_ParserHeadPacket(buffer, length, &next_posi, &left_len) == 0)
        // return 0;  
  
    char *PayloadData  = NULL;   
    int PayloadDataLen = 0;  
  
    while((unsigned int)left_len >= sizeof(PS_EACH_PACKET_START))  
    {           
        if(next_posi   
            && next_posi[0] == '\x00'   
            && next_posi[1] == '\x00'  
            && next_posi[2] == '\x01'  
            && next_posi[3] == '\xBA')  //如果是PS头包
        {  
            if(PS_ParserHeadPacket(&next_posi, &left_len) == 0)
                break;  
        }else 
        if(next_posi   
        && next_posi[0] == '\x00'   
        && next_posi[1] == '\x00'   
        && next_posi[2] == '\x01'   
        && next_posi[3] == '\xE0')  //如果是PES包
        {  
            //接着就是流包，说明是非i帧  
            if(PS_ParserPESPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen))  
            {  
                if(PayloadDataLen)  
                {   
                    memcpy(h264_buf, PayloadData, PayloadDataLen); 
                    h264_buf    += PayloadDataLen;  
                    (*h264_len) += PayloadDataLen; 
                    // for (int i = 0 ; i < *h264_len; ++i)
                    //     printf("%02x ", (unsigned char)*h264_buf[i]); 
                }  
            }  
            else   //说明最后一包H264数据了
            {  
                if(PayloadDataLen)  
                {  
                    memcpy(h264_buf, PayloadData, PayloadDataLen);  
                    h264_buf    += PayloadDataLen;  
                    (*h264_len) += PayloadDataLen;  
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
            if(PS_ParserMapPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen) == 0)  
                break;  
        }  
        else   //如果啥都不是
        {
            // printf("leflen:%d, length:%d\n", left_len,length);
            // for (int i = length - left_len ; i < length - 1; ++i)
            //     printf("%02x ", buffer[i]);  
            next_posi++;//往后走
            left_len--;
            // break;
        }  
    }  
     
    return *h264_len;  
}  
