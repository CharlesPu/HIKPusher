#ifndef _HC_EHOME_STREAM_H_
#define _HC_EHOME_STREAM_H_

#include "HCEHomePublic.h"



typedef struct tagNET_EHOME_NEWLINK_CB_MSG
{
    BYTE    szDeviceID[MAX_DEVICE_ID_LEN];   //设备标示符    
    LONG    iSessionID;     //设备分配给该取流会话的ID
    DWORD   dwChannelNo;    //设备通道号
    BYTE    byStreamType;   //0-主码流，1-子码流
    BYTE    byRes1[3];
    char   sDeviceSerial[NET_EHOME_SERIAL_LEN];    //设备序列号，数字序列号
    BYTE    byRes[112];
}NET_EHOME_NEWLINK_CB_MSG, *LPNET_EHOME_NEWLINK_CB_MSG;

typedef BOOL (CALLBACK *PREVIEW_NEWLINK_CB)(LONG lLinkHandle, NET_EHOME_NEWLINK_CB_MSG *pNewLinkCBMsg, void *pUserData);

typedef struct tagNET_EHOME_LISTEN_PREVIEW_CFG
{
    NET_EHOME_IPADDRESS struIPAdress; //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，多个网卡的情况下，默认为从操作系统获取到的第一个
    PREVIEW_NEWLINK_CB    fnNewLinkCB; //预览请求回调函数，当收到预览连接请求后，SDK会回调该回调函数。
    void*                pUser;        // 用户参数，在fnNewLinkCB中返回出来
    BYTE               byLinkMode;   //0：TCP，1：UDP 2: HRUDP方式
    BYTE               byRes[127];
}NET_EHOME_LISTEN_PREVIEW_CFG, *LPNET_EHOME_LISTEN_PREVIEW_CFG;

typedef struct tagNET_EHOME_PREVIEW_CB_MSG
{
    BYTE     byDataType;       //NET_DVR_SYSHEAD(1)-码流头，NET_DVR_STREAMDATA(2)-码流数据
    BYTE     byRes1[3];
    void    *pRecvdata;      //码流头或者数据
    DWORD   dwDataLen;      //数据长度
    BYTE     byRes2[128];
}NET_EHOME_PREVIEW_CB_MSG, *LPNET_EHOME_PREVIEW_CB_MSG;
typedef void(CALLBACK *PREVIEW_DATA_CB)( LONG  iPreviewHandle, NET_EHOME_PREVIEW_CB_MSG *pPreviewCBMsg,     void *pUserData);
typedef struct tagNET_EHOME_PREVIEW_DATA_CB_PARAM
{
    PREVIEW_DATA_CB    fnPreviewDataCB;    //数据回调函数
    void       *pUserData;         //用户参数, 在fnPreviewDataCB回调出来
    BYTE       byRes[128];          //保留
}NET_EHOME_PREVIEW_DATA_CB_PARAM, *LPNET_EHOME_PREVIEW_DATA_CB_PARAM;

NET_DVR_API BOOL CALLBACK NET_ESTREAM_Init();

NET_DVR_API BOOL CALLBACK NET_ESTREAM_Fini();

NET_DVR_API BOOL CALLBACK NET_ESTREAM_GetLastError();

NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetExceptionCallBack(DWORD dwMessage, HANDLE hWnd, void (CALLBACK* fExceptionCallBack)(DWORD dwType, LONG iUserID, LONG iHandle, void* pUser), void* pUser );

NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetLogToFile( LONG iLogLevel, char *strLogDir, BOOL bAutoDel );

//获取版本号
NET_DVR_API DWORD CALLBACK NET_ESTREAM_GetBuildVersion();

NET_DVR_API LONG CALLBACK NET_ESTREAM_StartListenPreview(LPNET_EHOME_LISTEN_PREVIEW_CFG pListenParam);

NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopListenPreview(LONG iListenHandle);

NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopPreview(LONG iPreviewHandle);

NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetPreviewDataCB(LONG iHandle, LPNET_EHOME_PREVIEW_DATA_CB_PARAM pStruCBParam);



#define    NET_EHOME_DEVICEID_LEN        256 //设备ID长度

typedef struct tagNET_EHOME_PLAYBACK_NEWLINK_CB_INFO
{
    char         szDeviceID[NET_EHOME_DEVICEID_LEN];   //设备标示符
    LONG         lSessionID;     //设备分配给该回放会话的ID，0表示无效
    DWORD        dwChannelNo;    //设备通道号，0表示无效
    char         sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];	//设备序列号，数字序列号
    BYTE         byRes[108];
}NET_EHOME_PLAYBACK_NEWLINK_CB_INFO, *LPNET_EHOME_PLAYBACK_NEWLINK_CB_INFO;


typedef BOOL (CALLBACK *PLAYBACK_NEWLINK_CB)(LONG lPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData);


typedef struct tagNET_EHOME_PLAYBACK_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struIPAdress;   //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，
    //多个网卡的情况下，默认为从操作系统获取到的第一个
    PLAYBACK_NEWLINK_CB fnNewLinkCB;    //回放新连接回调函数
    void*               pUserData;      //用户参数，在fnNewLinkCB中返回出来
    BYTE                byLinkMode;     //0：TCP，1：UDP (UDP保留)
    BYTE                byRes[127];
}NET_EHOME_PLAYBACK_LISTEN_PARAM, *LPNET_EHOME_PLAYBACK_LISTEN_PARAM;

typedef struct tagNET_EHOME_PLAYBACK_DATA_CB_INFO
{
    DWORD   dwType;                    //类型 0-头信息 1-码流数据
    BYTE     *pData;                    //数据指针
    DWORD     dwDataLen;                //数据长度
    BYTE    byRes[128];                //保留
}NET_EHOME_PLAYBACK_DATA_CB_INFO, *LPNET_EHOME_PLAYBACK_DATA_CB_INFO;


typedef BOOL (CALLBACK *PLAYBACK_DATA_CB)(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData);


typedef struct tagNET_EHOME_PLAYBACK_DATA_CB_PARAM
{
    PLAYBACK_DATA_CB    fnPlayBackDataCB;        //数据回调函数
    void*                pUserData;                //用户参数, 在fnPlayBackDataCB回调出来
    BYTE                byStreamFormat;            //码流封装格式：0-PS 1-RTP 
    BYTE                 byRes[127];                //保留
}NET_EHOME_PLAYBACK_DATA_CB_PARAM, *LPNET_EHOME_PLAYBACK_DATA_CB_PARAM;

#define    EHOME_PREVIEW_EXCEPTION        0x102    //预览取流异常
#define    EHOME_PLAYBACK_EXCEPTION    0x103    //回放取流异常
#define    EHOME_AUDIOTALK_EXCEPTION    0x104    //语音对讲取流异常

#define NET_EHOME_SYSHEAD            1    //系统头数据
#define NET_EHOME_STREAMDATA        2    //视频流数据

NET_DVR_API LONG CALLBACK NET_ESTREAM_StartListenPlayBack(LPNET_EHOME_PLAYBACK_LISTEN_PARAM pListenParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetPlayBackDataCB(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_PARAM *pDataCBParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopPlayBack(LONG iPlayBackLinkHandle);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopListenPlayBack(LONG iPlaybackListenHandle);

//--------------------------------------------------------------------------------------------------------------
#define NET_EHOME_DEVICEID_LEN      256 //设备ID长度
#define NET_EHOME_SERIAL_LEN        12

typedef struct tagNET_EHOME_VOICETALK_NEWLINK_CB_INFO
{
    BYTE    szDeviceID[NET_EHOME_DEVICEID_LEN/*256*/];   //设备标示符    
    DWORD   dwEncodeType; // //SDK赋值,当前对讲设备的语音编码类型,0- OggVorbis，1-G711U，2-G711A，3-G726，4-AAC，5-MP2L2，6-PCM
    char    sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];    //设备序列号，数字序列号
    DWORD   dwAudioChan; //对讲通道
    LONG    lSessionID;     //设备分配给该回放会话的ID，0表示无效
    BYTE    byRes[128];
} NET_EHOME_VOICETALK_NEWLINK_CB_INFO, *LPNET_EHOME_VOICETALK_NEWLINK_CB_INFO;


typedef BOOL (CALLBACK *VOICETALK_NEWLINK_CB)(LONG lHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData);

typedef struct tagNET_EHOME_LISTEN_VOICETALK_CFG
{
    NET_EHOME_IPADDRESS struIPAdress;   //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，
    //多个网卡的情况下，默认为从操作系统获取到的第一个
    VOICETALK_NEWLINK_CB     fnNewLinkCB;    //新连接回调函数
    void*          pUser;        //用户参数，在fnNewLinkCB中返回出来
    BYTE           byRes[128];
}NET_EHOME_LISTEN_VOICETALK_CFG, *LPNET_EHOME_LISTEN_VOICETALK_CFG;

typedef struct tagNET_EHOME_VOICETALK_DATA_CB_INFO
{
    BYTE         *pData;               //数据指针
    DWORD        dwDataLen;            //数据长度
    BYTE         byRes[128];           //保留
}NET_EHOME_VOICETALK_DATA_CB_INFO, *LPNET_EHOME_VOICETALK_DATA_CB_INFO;

typedef BOOL (CALLBACK *VOICETALK_DATA_CB)(LONG lHandle, NET_EHOME_VOICETALK_DATA_CB_INFO *pDataCBInfo, void* pUserData);

typedef struct tagNET_EHOME_VOICETALK_DATA_CB_PARAM
{
    VOICETALK_DATA_CB    fnVoiceTalkDataCB;    //数据回调函数
    void       *pUserData;         //用户参数, 在fnVoiceTalkDataCB回调出来
    BYTE       byRes[128];          //保留
}NET_EHOME_VOICETALK_DATA_CB_PARAM, *LPNET_EHOME_VOICETALK_DATA_CB_PARAM;

typedef struct tagNET_EHOME_VOICETALK_DATA
{
    BYTE         *pSendBuf;            //音频数据缓冲区
    DWORD     dwDataLen;            //音频数据长度
    BYTE        byRes[128];            //保留
}NET_EHOME_VOICETALK_DATA, *LPNET_EHOME_VOICETALK_DATA;

NET_DVR_API LONG CALLBACK NET_ESTREAM_StartListenVoiceTalk(LPNET_EHOME_LISTEN_VOICETALK_CFG pListenParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopListenVoiceTalk(LONG lListenHandle);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetVoiceTalkDataCB(LONG lHandle, LPNET_EHOME_VOICETALK_DATA_CB_PARAM pStruCBParam);
NET_DVR_API LONG CALLBACK NET_ESTREAM_SendVoiceTalkData (LONG lHandle, LPNET_EHOME_VOICETALK_DATA pVoicTalkData);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopVoiceTalk(LONG lHandle);



NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType, void* const lpInBuff);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType, void *lpOutBuff);





#endif //_HC_EHOME_STREAM_H_
