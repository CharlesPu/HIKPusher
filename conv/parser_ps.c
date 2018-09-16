/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser_ps.h"
#include "config.h"
/*************************************************
@Description: 处理PS流的包头
@Input: Pack-流起始位置和处理完之后指向的包的位置    length-流目前长度和处理完之后包剩下的长度
@Output: 
@Return: 0 - success, else - fail
@Others: 
*************************************************/
int PS_ParserHeadPacket(char** Pack, int *length)  
{  
#ifdef PS_PRINT_HEAD_PACK_HEADER
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, 
        (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif
    //通过 00 00 01 ba头的第14个字节的最后3位来确定头部填充了多少字节  
    PS_HEAD_PACKET_HEADER *PS_head_pack = (PS_HEAD_PACKET_HEADER *) (*Pack);  
    unsigned char pack_stuffing_length = PS_head_pack->stuffing_len & '\x07';  
  
    *length = (*length) - sizeof(PS_HEAD_PACKET_HEADER) - pack_stuffing_length;//减去头和填充的字节  
    *Pack   = (*Pack) + sizeof(PS_HEAD_PACKET_HEADER) + pack_stuffing_length;  

#ifdef PS_PRINT_HEAD_PACK_HEADER 
    printf("[%s]leftlen:%d\n", __FUNCTION__, *length);  
    if(*length > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, 
            (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif
    return 0;  
}  

/*************************************************
@Description: 处理PS流的映射流包头
@Input: Pack-流起始位置和处理完之后指向的包的位置    length-流目前长度和处理完之后包剩下的长度
@Output: 
@Return: 0 - success, else - fail
@Others: 
*************************************************/  
int PS_ParserMapPacket(char** Pack, int* length, char **PayloadData, int *PayloadDataLen)  
{ 
#ifdef PS_PRINT_MAP_PACK_HEADER 
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, 
        (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif     
    PS_MAP_PACKET_HEADER* PSM_pack = (PS_MAP_PACKET_HEADER*) (*Pack);  
  
    //no payload  
    *PayloadData = 0;  
    *PayloadDataLen = 0;  
      
    if((unsigned int)(*length) < sizeof(PS_MAP_PACKET_HEADER)) return 1;  
  
    PS_2BYTES_LENGTH data_length;  
    data_length.byte[0] = PSM_pack->pack_len.byte[1];  
    data_length.byte[1] = PSM_pack->pack_len.byte[0];  

    *length = (*length) - data_length.length - sizeof(PS_MAP_PACKET_HEADER);  
    *Pack = (*Pack) + data_length.length + sizeof(PS_MAP_PACKET_HEADER);  
#ifdef PS_PRINT_MAP_PACK_HEADER  
    printf("[%s]leftlen:%d\n", __FUNCTION__, *length);  
    if(*length > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, 
            (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif 
    return 0;  
}  
 
/*************************************************
@Description: 处理PES包头
@Input: Pack-流起始位置和处理完之后指向的包的位置    length-流目前长度和处理完之后包剩下的长度
        PayloadData-h264裸流的指向位置    PayloadDataLen-h264裸流的长度
@Output: 
@Return: 0 - success, else - fail
@Others: 所有传入传出参数都为指针，并不会分配新的内存空间！内存空间存在于函数之外
*************************************************/ 
int PS_ParserPESPacket(char** Pack, int *length, char **PayloadData, int *PayloadDataLen)  
{  
#ifdef PS_PRINT_PES_PACK_HEADER  
    printf("[%s]now :%02x %02x %02x %02x\n", __FUNCTION__, 
        (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif
    PES_PACKET_HEADER* PES_pack = (PES_PACKET_HEADER*) (*Pack);  
    //初始化清空！！
    *PayloadData    = NULL;  
    *PayloadDataLen = 0;  

    if((unsigned int)(*length) < sizeof(PES_PACKET_HEADER) - 1) return 1;  
      
    PS_2BYTES_LENGTH data_length;  
    data_length.byte[0] = PES_pack->pack_len.byte[1];  
    data_length.byte[1] = PES_pack->pack_len.byte[0];  

    *PayloadDataLen = data_length.length - 2 - 1 - PES_pack->stuffing_len;  
    int header_len = 9 + PES_pack->stuffing_len;  
    if(*PayloadDataLen > 0)   //只需指向位置，内存空间已经在外面分配了!!
        *PayloadData =  (*Pack) + header_len;
          
    *length = (*length) - (header_len + *PayloadDataLen);    
    *Pack = (*Pack) + (header_len + *PayloadDataLen);  
 
#ifdef PS_PRINT_PES_PACK_HEADER  
    printf("[%s]payloadlen:%d, leftlen:%d\n", __FUNCTION__, *PayloadDataLen, *length);  
    if(*length > 0)
        printf("[%s]next:%02x %02x %02x %02x\n", __FUNCTION__, 
            (unsigned char)(*Pack)[0], (unsigned char)(*Pack)[1], (unsigned char)(*Pack)[2], (unsigned char)(*Pack)[3]);  
#endif 
    return 0;  
}  