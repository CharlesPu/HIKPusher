/*******************************
@@Author     : Charles
@@Date       : 2018-12-27
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser_ps.h"
#include "parser_h264.h"
#include "config.h"

/******************************
**  第一链：PS解析器
**  传入为RTP原始数据流
**  处理后传出为一个完整的PES包荷载数据，这个荷载数据实际为n(n>1)个NALU，可能是断的，
    但是第一链只保证PES荷载数据的完整性，而不保证内容含义的完整性，所以需要第二链
    自行保证n个NALU的截断和拼接
**  链缓冲区pespack_buf用来缓冲残缺的PES荷载数据，直到得到完整的一个为止
******************************/
int PS_Parser(struct _ipc *ipc, char *rtp_pack, int length)
{    
    /***************************************残余项解析任务 开始***************************************/
    //残余项长度由PES包头中长度字段得知
    //由于其它历史遗留信息的存在，接下来的判断过程只需根据pespack_left_len成员判断即可
    if (ipc->pespack_left_len) //如有残余长度存在，则直接尝试拼接剩下的
    {
        int real_cp_len = ipc->pespack_left_len > length ? length : ipc->pespack_left_len;//取小值
        memcpy(ipc->pespack_buf + ipc->pespack_buf_len, rtp_pack, real_cp_len);
        ipc->pespack_left_len -= real_cp_len;
        ipc->pespack_buf_len += real_cp_len;
        length -= real_cp_len;
        rtp_pack += real_cp_len;

        if (!(ipc->pespack_left_len))//若拼接后发现已经得到了一个完整的PES荷载数据，则直接送入下一链
        {
            H264_Parser(ipc, ipc->pespack_buf, ipc->pespack_buf_len);
            memset(ipc->pespack_buf, 0, PESPACK_BUF_MAX_SIZE);
            ipc->pespack_buf_len = 0;
        }//若还是没得到完整的，说明还是有残余项，后面还是要进入此代码
    }
    /************************************残余项解析任务 结束******************************************/

    //处理完上一次的残余项之后正式开始后续PES解析
    int left_len        = length;  
    char *next_posi     = rtp_pack;   
    char *PayloadData   = NULL;   
    int PayloadDataLen  = 0;
    while(left_len >= 4)  // >= sizeof(PS_EACH_PACKET_START)
    {    
        if(next_posi   
        && next_posi[0] == '\x00'   
        && next_posi[1] == '\x00'  
        && next_posi[2] == '\x01') 
        {
            switch (next_posi[3])
            {
                case '\xBA':
                    if(PS_ParserHeadPacket(&next_posi, &left_len))
                        return 1;
                    break;
                case '\xBC':
                    if(PS_ParserMapPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen))
                        return 2;
                    break;
                case '\xE0':
                    if(!PS_ParserPESPacket(&next_posi, &left_len, &PayloadData, &PayloadDataLen))  
                    {  
                        if (left_len < 0)//如果断了，则存储已有数据，后续需要交由 残余项解析任务 完成
                        {
                            ipc->pespack_left_len = -left_len;
                            int real_cp_len = PayloadDataLen - (-left_len);
                            memcpy(ipc->pespack_buf + ipc->pespack_buf_len, PayloadData, real_cp_len);
                            ipc->pespack_buf_len += real_cp_len;
                        }else//最完美的情况下是没断，则直接得到一个完整的PES荷载数据，直接送入下一链
                        {
                            H264_Parser(ipc, PayloadData, PayloadDataLen);
                            ipc->pespack_left_len = 0;
                            memset(ipc->pespack_buf, 0, PESPACK_BUF_MAX_SIZE);
                            ipc->pespack_buf_len = 0;
                        }
                    }  
                    else //payload之前就断了，那么直接放弃此包
                        return 3;
                    break;
                default:
                    next_posi++;//往后走
                    left_len--;
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

    if(((*length) > 0 ? (*length) : -(*length)) < sizeof(PES_PACKET_HEADER) - 1) return 1;  //包头9个字节断了
      
    PS_2BYTES_LENGTH data_length;  
    data_length.byte[0] = PES_pack->pack_len.byte[1];  
    data_length.byte[1] = PES_pack->pack_len.byte[0];  

    *PayloadDataLen = data_length.length - 2 - 1 - PES_pack->stuffing_len;  
    int header_len = 9 + PES_pack->stuffing_len;  
    if(*PayloadDataLen > 0)   //只需指向位置，内存空间已经在外面分配了!!
        *PayloadData =  (*Pack) + header_len;
    if (((*length) > 0 ? (*length) : -(*length)) < header_len) return 2;  //附加长度断了
  
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