/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ps-h264.h"
#include "config.h"

/*************************************************
@Description: 处理PS流的包头
@Input: Pack-流起始位置   length-流目前长度    NextPack-处理完之后指向的包的位置    leftlength-处理完之后包剩下的长度
@Output: 
@Return: 流剩下长度
@Others: 
*************************************************/
int PS_ProcessHeadPacket(char* Pack, int length, char **NextPack, int *leftlength)  
{  
#ifdef PS_PRINT_HEAD_PACK_HEADER
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)Pack[0], (unsigned char)Pack[1], (unsigned char)Pack[2], (unsigned char)Pack[3]);  
#endif
    //通过 00 00 01 ba头的第14个字节的最后3位来确定头部填充了多少字节  
    PS_HEAD_PACKET_HEADER *PS_head_pack = (PS_HEAD_PACKET_HEADER *) Pack;  
    unsigned char pack_stuffing_length = PS_head_pack->stuffing_len & '\x07';  
  
    *leftlength = length - sizeof(PS_HEAD_PACKET_HEADER) - pack_stuffing_length;//减去头和填充的字节  
    *NextPack   = Pack + sizeof(PS_HEAD_PACKET_HEADER) + pack_stuffing_length;  

#ifdef PS_PRINT_HEAD_PACK_HEADER 
    printf("[%s]leftlen:%d\n", __FUNCTION__, *leftlength);  
    if(*leftlength > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)(*NextPack)[0], (unsigned char)(*NextPack)[1], (unsigned char)(*NextPack)[2], (unsigned char)(*NextPack)[3]);  
#endif
    if(*leftlength < 4) return 0;
    return *leftlength;  
}  

/*************************************************
@Description: 处理PS流的映射流包头
@Input: Pack-流起始位置   length-流目前长度    NextPack-处理完之后指向的包的位置    leftlength-处理完之后包剩下的长度
@Output: 
@Return: 流剩下长度
@Others: 
*************************************************/  
int PS_ProcessMapPacket(char* Pack, int length, char **NextPack, int *leftlength, char **PayloadData, int *PayloadDataLen)  
{ 
#ifdef PS_PRINT_MAP_PACK_HEADER 
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)Pack[0], (unsigned char)Pack[1], (unsigned char)Pack[2], (unsigned char)Pack[3]);  
#endif     
    PS_MAP_PACKET_HEADER* PSM_pack = (PS_MAP_PACKET_HEADER*) Pack;  
  
    //no payload  
    *PayloadData = 0;  
    *PayloadDataLen = 0;  
      
    if((unsigned int)length < sizeof(PS_MAP_PACKET_HEADER)) return 0;  
  
    PS_2BYTES_LENGTH data_length;  
    data_length.byte[0] = PSM_pack->pack_len.byte[1];  
    data_length.byte[1] = PSM_pack->pack_len.byte[0];  

    *leftlength = length - data_length.length - sizeof(PS_MAP_PACKET_HEADER);  
    *NextPack = Pack + data_length.length + sizeof(PS_MAP_PACKET_HEADER);  
#ifdef PS_PRINT_MAP_PACK_HEADER  
    printf("[%s]leftlen:%d\n", __FUNCTION__, *leftlength);  
    if(*leftlength > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)(*NextPack)[0], (unsigned char)(*NextPack)[1], (unsigned char)(*NextPack)[2], (unsigned char)(*NextPack)[3]);  
#endif
    if(*leftlength <= 0) return 0; 
    return *leftlength;  
}  
 
/*************************************************
@Description: 处理PES包头
@Input: Pack-流起始位置   length-流目前长度    NextPack-处理完之后指向的包的位置    leftlength-处理完之后包剩下的长度
        PayloadData-h264裸流的指向位置    PayloadDataLen-h264裸流的长度
@Output: 
@Return: 流剩下长度
@Others: 
*************************************************/ 
int PS_ProcessPESPacket(char* Pack, int length, char **NextPack, int *leftlength, char **PayloadData, int *PayloadDataLen)  
{  
#ifdef PS_PRINT_PES_PACK_HEADER  
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)Pack[0], (unsigned char)Pack[1], (unsigned char)Pack[2], (unsigned char)Pack[3]);  
#endif
    PES_PACKET_HEADER* PES_pack = (PES_PACKET_HEADER*) Pack;  
    //初始化清空！！
    *PayloadData    = NULL;  
    *PayloadDataLen = 0;  

    if((unsigned int)length < sizeof(PES_PACKET_HEADER) - 1) return 0;  
      
    PS_2BYTES_LENGTH data_length;  
    data_length.byte[0] = PES_pack->pack_len.byte[1];  
    data_length.byte[1] = PES_pack->pack_len.byte[0];  
  
    *PayloadDataLen = data_length.length - 2 - 1 - PES_pack->stuffing_len;  
    if(*PayloadDataLen > 0)   //只需指向位置，内存空间已经在外面分配了!!
    {
        *PayloadData =  Pack + sizeof(PES_PACKET_HEADER) - 1 + PES_pack->stuffing_len;
    }
          
    *leftlength = length - data_length.length - sizeof(PS_EACH_PACKET_START) - sizeof(PS_2BYTES_LENGTH);    
    *NextPack = Pack + sizeof(PS_EACH_PACKET_START) + sizeof(PS_2BYTES_LENGTH) + data_length.length;  
 
 #ifdef PS_PRINT_PES_PACK_HEADER  
    printf("[%s]payloadlen:%d, leftlen:%d\n", __FUNCTION__, *PayloadDataLen, *leftlength);  
    if(*leftlength > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, (unsigned char)(*NextPack)[0], (unsigned char)(*NextPack)[1], (unsigned char)(*NextPack)[2], (unsigned char)(*NextPack)[3]);  
#endif 
    if(*leftlength <= 0) return 0; 
    return *leftlength;  
}  

/*************************************************
@Description: 从PS流中提取出H264
@Input: buffer-流起始位置   length-流目前长度    h264_buf-H264需要存储的位置    h264_len-H264的长度
@Output: 
@Return: 读取到的H264长度
@Others: 
*************************************************/ 
int PS_GetH264(char* buffer, int length, char *h264_buf, int *h264_len)  
{  
    int left_len    = length;  
    char *next_posi = buffer;  

    // if(PS_ProcessHeadPacket(buffer, length, &next_posi, &left_len) == 0)
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
            if(PS_ProcessHeadPacket(next_posi, left_len, &next_posi, &left_len) == 0)
                break;  
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
            if(PS_ProcessMapPacket(next_posi, left_len, &next_posi, &left_len, &PayloadData, &PayloadDataLen) == 0)  
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
