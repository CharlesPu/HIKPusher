/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __PARSER_PS_H__
#define __PARSER_PS_H__

typedef union //后面要注意大端存放
{  
    unsigned short int  length;  
    unsigned char       byte[2];  
}PS_2BYTES_LENGTH; 
//每个PS包的起始码
typedef struct  
{  
    unsigned char start_code[3];  // start_code :00 00 01 
    unsigned char stream_id[1];  //  流标识 ：ba/bb/bc/e0
}PS_EACH_PACKET_START;  
//PS包头的起始码
typedef struct   
{  
    PS_EACH_PACKET_START 	pack_start;// 4个字节的包头起始码
    unsigned char 			buf[9];  //包头不重要的9个字节
    unsigned char 			stuffing_len;  //扩展字节长度
}PS_HEAD_PACKET_HEADER;  
//映射流包头
typedef struct   
{  
    PS_EACH_PACKET_START 	pack_start;  
    PS_2BYTES_LENGTH 		pack_len; //we must do exchange   
}PS_MAP_PACKET_HEADER;   
//PES包头,注意长度是uint两字节对齐！！因为PS_2BYTES_LENGTH中uint的存在！！！所以stuffing_len占2字节！
typedef struct   
{  
    PS_EACH_PACKET_START    pack_start;  
    PS_2BYTES_LENGTH  		pack_len;//we must do exchange  
    unsigned char           pack_info[2];  // 8c xx
    unsigned char       	stuffing_len;  //包头中扩展的长度,实际占2字节
}PES_PACKET_HEADER;  
  
int PS_ParserHeadPacket(char** Pack, int *length);
int PS_ParserMapPacket(char** Pack, int *length, char **PayloadData, int *PayloadDataLen);
int PS_ParserPESPacket(char** Pack, int *length, char **PayloadData, int *PayloadDataLen);

#endif