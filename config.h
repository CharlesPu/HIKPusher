#ifndef __CONFIG_H__
#define __CONFIG_H__

/*DEBUG*/
// #define PRINT_H264_NALU_RD
#define PRINT_RTMP_SEND_SIZE
// #define PRINT_REQ_RECV_RAW
// #define PRINT_REQ_LINK

/**RTMP**/
#define RTMP_URL "rtmp://localhost:1935/blade_test/"
#define RTMP_CONNECTION_TIMEOUT 5  //s
#define RTMP_SEND_INTVL 20  //ms

/*H264*/
#define PESPACK_BUF_MAX_SIZE 262144
#define H264PACK_BUF_MAX_SIZE 262144
/*******************************************************/
/*NET_ESTREAM*/
#define NET_ESTREAM_IP "0.0.0.0"
#define NET_ESTREAM_PUBLIC_IP "x.x.x.x"
#define NET_ESTREAM_PORT 8003
/*NET_ECMS*/
#define NET_ECMS_PORT 7660

//0- TCP方式，1- UDP方式 
#define LINK_MODE_TCP 0
#define LINK_MODE_UDP 1
//码流类型：0- 主码流，1- 子码流, 2- 第三码流
#define STREAM_TYPE_MAIN  0
#define STREAM_TYPE_SUB   1
#define STREAM_TYPE_THIRD 2
//通道号
#define CHANNEL_ID 1

/*request listen server*/
#define REQ_SRV_PORT 9001
#define REQ_SRV_CLIE_MAX_NUM 10
#define REQ_SRV_BUF_LEN 1024

/*IPCS*/
#define IPCS_MAX_NUM 4096
#define IPCS_EPOLL_WAIT_TIME 5000  //ms  
#define IPCS_HEARTBEAT_INVL 4000000  //us  // 4s cause the delay of the internet
//states
#define IPCS_PUSHING_STREAM 1
#define IPCS_NOT_PUSHING_STREAM 0
#define IPCS_ONLINE 1
#define IPCS_OFFLINE 0

/*log*/
// #define LOG_SAVE
#define LOG_CONTENT_MAX_SIZE 256
#define LOG_OUT_PATH "./logs/"

#endif
