#ifndef _HC_EHOME_CMS_H_
#define _HC_EHOME_CMS_H_

#include "HCEHomePublic.h"

//初始化，反初始化
NET_DVR_API BOOL  CALLBACK NET_ECMS_Init();
NET_DVR_API BOOL  CALLBACK NET_ECMS_Fini();

//获取错误码
NET_DVR_API DWORD CALLBACK NET_ECMS_GetLastError();

//获取版本号
NET_DVR_API DWORD CALLBACK NET_ECMS_GetBuildVersion();

//开启关闭监听
typedef enum tagNET_EHOME_REGISTER_TYPE{
    ENUM_UNKNOWN = -1,   
    ENUM_DEV_ON = 0,             //设备上线回调
    ENUM_DEV_OFF,                //设备下线回调
    ENUM_DEV_ADDRESS_CHANGED     //设备地址发生变化
}NET_EHOME_REGISTER_TYPE;

typedef BOOL (CALLBACK * DEVICE_REGISTER_CB)(LONG lUserID, DWORD dwDataType, void *pOutBuffer, DWORD dwOutLen, 
                                                 void *pInBuffer, DWORD dwInLen, void *pUser);

typedef struct tagNET_EHOME_CMS_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struAddress;  //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，多个网卡的情况下，默认为从操作系统获取到的第一个
    DEVICE_REGISTER_CB  fnCB; //报警信息回调函数
    void * pUserData;   //用户数据
    BYTE  byRes[32]; 
}NET_EHOME_CMS_LISTEN_PARAM, *LPNET_EHOME_CMS_LISTEN_PARAM;

//开启关闭监听
NET_DVR_API LONG CALLBACK NET_ECMS_StartListen(LPNET_EHOME_CMS_LISTEN_PARAM lpCMSListenPara);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopListen(LONG iHandle);
//注销设备
NET_DVR_API BOOL CALLBACK NET_ECMS_ForceLogout(LONG lUserID);
NET_DVR_API BOOL CALLBACK NET_ECMS_SetLogToFile(DWORD iLogLevel, char *strLogDir, BOOL bAutoDel);

#define FIRMWARE_VER_LEN 24
#define CODE_LEN   8
typedef struct tagNET_EHOME_DEV_REG_INFO
{
    DWORD  dwSize; 
    DWORD  dwNetUnitType;            //根据EHomeSDK协议预留，目前没有意义
    BYTE   byDeviceID[MAX_DEVICE_ID_LEN]; //设备ID
    BYTE   byFirmwareVersion[24];    //固件版本
    NET_EHOME_IPADDRESS struDevAdd;         //设备注册上来是，设备的本地地址
    DWORD  dwDevType;                  //设备类型
    DWORD  dwManufacture;              //设备厂家代码
    BYTE   byPassWord[32];             //设备登陆CMS的密码，由用户自行根据需求进行验证
    BYTE   sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];    //设备序列号，数字序列号
    BYTE   byRes[52];
}NET_EHOME_DEV_REG_INFO, *LPNET_EHOME_DEV_REG_INFO;

typedef struct tagNET_EHOME_DEV_REG_INFO_V12
{
    NET_EHOME_DEV_REG_INFO struRegInfo;
    NET_EHOME_IPADDRESS struRegAddr;         //设备注册的服务器地址
    BYTE   byRes[256];
}NET_EHOME_DEV_REG_INFO_V12, *LPNET_EHOME_DEV_REG_INFO_V12;

typedef struct tagNET_EHOME_BLACKLIST_SEVER
{
    NET_EHOME_IPADDRESS struAdd;          //服务器地址
    BYTE    byServerName[NAME_LEN/*32*/];       //服务器名称
    BYTE    byUserName[NAME_LEN/*32*/];             //用户名
    BYTE    byPassWord[NAME_LEN/*32*/];             //密码
    BYTE   byRes[64];
}NET_EHOME_BLACKLIST_SEVER, *LPNET_EHOME_BLACKLIST_SEVER;

typedef struct tagNET_EHOME_SERVER_INFO
{
    DWORD                  dwSize;
    DWORD                  dwKeepAliveSec;            //心跳间隔（单位：秒,0:默认为15S）
    DWORD                  dwTimeOutCount;         //心跳超时次数（0：默认为6）
    NET_EHOME_IPADDRESS         struTCPAlarmSever;      //报警服务器地址（TCP协议）
    NET_EHOME_IPADDRESS         struUDPAlarmSever;        //报警服务器地址（UDP协议）
    DWORD                  dwAlarmServerType;        //报警服务器类型0-只支持UDP协议上报，1-支持UDP、TCP两种协议上报
    NET_EHOME_IPADDRESS         struNTPSever;            //NTP服务器地址
    DWORD                  dwNTPInterval;            //NTP校时间隔（单位：秒）
    NET_EHOME_IPADDRESS         struPictureSever;       //图片服务器地址
    DWORD                  dwPicServerType;        //图片服务器类型图片服务器类型，1-VRB图片服务器，0-Tomcat图片服务,2-云存储3,3-KMS
    NET_EHOME_BLACKLIST_SEVER   struBlackListServer;    //黑名单服务器
    BYTE                   byRes[128];
}NET_EHOME_SERVER_INFO, *LPNET_EHOME_SERVER_INFO;


//-----------------------------------------------------------------------------------------------------------
//语音对讲

typedef void(CALLBACK *fVoiceDataCallBack)(LONG iVoiceHandle, char *pRecvDataBuffer, DWORD dwBufSize, DWORD dwEncodeType, BYTE byAudioFlag, void* pUser);

typedef struct tagNET_EHOME_VOICETALK_PARA
{
    BOOL           bNeedCBNoEncData; //需要回调的语音类型：0-编码后语音，1-编码前语音（语音转发时不支持）
    fVoiceDataCallBack  cbVoiceDataCallBack; //用于回调音频数据的回调函数
    DWORD          dwEncodeType;    //SDK赋值,SDK的语音编码类型,0- OggVorbis，1-G711U，2-G711A，3-G726，4-AAC，5-MP2L2，6-PCM
    void*          pUser;    //用户参数
    BYTE           byVoiceTalk;    //0-语音对讲,1-语音转发
    BYTE           byDevAudioEnc;  //输出参数，设备的音频编码方式 0- OggVorbis，1-G711U，2-G711A，3-G726，4-AAC，5-MP2L2，6-PCM
    BYTE           byRes[62];//Reserved, set as 0. 0
} NET_EHOME_VOICETALK_PARA,*LPNET_EHOME_VOICETALK_PARA;

typedef struct tagNET_EHOME_VOICE_TALK_IN
{
    DWORD dwVoiceChan;                   //通道号
    NET_EHOME_IPADDRESS struStreamSever; //流媒体地址
    BYTE  byRes[128];
}NET_EHOME_VOICE_TALK_IN, *LPNET_EHOME_VOICE_TALK_IN;

typedef struct tagNET_EHOME_VOICE_TALK_OUT
{
    LONG  lSessionID;
    BYTE  byRes[128];
}NET_EHOME_VOICE_TALK_OUT, *LPNET_EHOME_VOICE_TALK_OUT;

typedef struct tagNET_EHOME_PUSHVOICE_IN
{
    DWORD dwSize;
    LONG  lSessionID; 
    BYTE  byRes[128];
}NET_EHOME_PUSHVOICE_IN, *LPNET_EHOME_PUSHVOICE_IN;

typedef struct tagNET_EHOME_PUSHVOICE_OUT
{
    DWORD dwSize;
    BYTE  byRes[128];
}NET_EHOME_PUSHVOICE_OUT, *LPNET_EHOME_PUSHVOICE_OUT;


NET_DVR_API LONG CALLBACK  NET_ECMS_StartVoiceTalk(LONG lUserID, DWORD dwVoiceChan, 
                                                   const NET_EHOME_VOICETALK_PARA    *pVoiceTalkPara);

NET_DVR_API BOOL CALLBACK   NET_ECMS_StartVoiceWithStmServer(LONG lUserID, NET_EHOME_VOICE_TALK_IN *lpVoiceTalkIn, NET_EHOME_VOICE_TALK_OUT *lpVoiceTalkOut);
NET_DVR_API BOOL CALLBACK   NET_ECMS_StartPushVoiceStream(LONG lUserID, LPNET_EHOME_PUSHVOICE_IN lpPushParamIn, LPNET_EHOME_PUSHVOICE_OUT lpPushParamOut);
NET_DVR_API BOOL CALLBACK   NET_ECMS_StopVoiceTalk(LONG iVoiceHandle);
NET_DVR_API BOOL CALLBACK   NET_ECMS_StopVoiceTalkWithStmServer(LONG lUserID, LONG lSessionID);
NET_DVR_API BOOL CALLBACK   NET_ECMS_SendVoiceTransData(LONG iVoiceHandle, const char *pSendBuf, DWORD dwBufSize);

bool NET_EHOME_ClientAudioStart(fVoiceDataCallBack cbVoiceDataCallBack, void *pUser);
//---------------------------------------------------------------------------------------------------------

typedef    struct    tagNET_EHOME_CONFIG
{
    void*        pCondBuf;    //[in]，条件数据指针，如表示通道号等
    DWORD        dwCondSize; //[in]，pCondBuf指向的数据大小
    void*        pInBuf;        //[in]，设置时需要用到，指向结构体的指针
    DWORD        dwInSize;    //[in], pInBuf指向的数据大小
    void*        pOutBuf;        //[out]，获取时需要用到，指向结构体的指针，内存由上层分配
    DWORD        dwOutSize;    //[in]，获取时需要用到，表示pOutBuf指向的内存大小， 
    BYTE         byRes[40];    //保留
}NET_EHOME_CONFIG, *LPNET_EHOME_CONFIG;

typedef struct tagNET_EHOME_XML_CFG 
{
    void*      pCmdBuf;    //字符串格式命令
    DWORD      dwCmdLen;   //pCmdBuf长度
    void*      pInBuf;     //输入数据
    DWORD      dwInSize;   //输入数据长度
    void*      pOutBuf;    //输出缓冲
    DWORD      dwOutSize;  //输出缓冲区长度
    DWORD      dwSendTimeOut;  //数据发送超时时间,单位ms，默认5s
    DWORD      dwRecvTimeOut;  //数据接收超时时间,单位ms，默认5s
    void*      pStatusBuf;     //返回的状态参数(XML格式),如果不需要,可以置NULL
    DWORD      dwStatusSize;   //状态缓冲区大小(内存大小)
    BYTE       byRes[24];
}NET_EHOME_XML_CFG, *LPNET_EHOME_XML_CFG;

#define    MAX_SERIALNO_LEN        128    //序列号最大长度
#define    MAX_PHOMENUM_LEN        32    //手机号码最大长度
#define    MAX_DEVICE_NAME_LEN     32//设备名称长度

typedef struct tagNET_DVR_DVR_TYPE_NAME
{
    DWORD dwDVRType;
    char byDevName[24];  
}NET_DVR_TYPE_NAME;

static const NET_DVR_TYPE_NAME DVRTypeName[] = 
{
    {0,"UNKNOWN TYPE"},      /*对尚未定义的dvr类型返回NETRET_DVR*/
    {1,"DVR"},                    /*DVR*/    
    {2,"ATMDVR"},                /*atm dvr*/
    {3,"DVS"},                /*DVS*/
    {4,"DEC"},                /* 6001D */
    {5,"ENC_DEC"},                /* 6001F */
    {6,"DVR_HC"},                /*8000HC*/
    {7,"DVR_HT"},                /*8000HT*/
    {8,"DVR_HF"},                /*8000HF*/
    {9,"DVR_HS"},                /* 8000HS DVR(no audio) */
    {10,"DVR_HTS"},              /* 8016HTS DVR(no audio) */
    {11,"DVR_HB"},              /* HB DVR(SATA HD) */
    {12,"DVR_HCS"},              /* 8000HCS DVR */
    {13,"DVS_A"},              /* 带ATA硬盘的DVS */
    {14,"DVR_HC_S"},              /* 8000HC-S */
    {15,"DVR_HT_S"},              /* 8000HT-S */
    {16,"DVR_HF_S"},              /* 8000HF-S */
    {17,"DVR_HS_S"},              /* 8000HS-S */
    {18,"ATMDVR_S"},              /* ATM-S */
    {19,"DVR_7000H"},                /*7000H系列*/
    {20,"DEC_MAT"},              /*多路解码器*/
    {21,"DVR_MOBILE"},                /* mobile DVR */                 
    {22,"DVR_HD_S"},              /* 8000HD-S */
    {23,"DVR_HD_SL"},                /* 8000HD-SL */
    {24,"DVR_HC_SL"},                /* 8000HC-SL */
    {25,"DVR_HS_ST"},                /* 8000HS_ST */
    {26,"DVS_HW"},              /* 6000HW */
    {27,"DS630X_D"},             /* 多路解码器 */
    {28,"DS640X_HD"},                /*640X高清解码器*/
    {29,"DS610X_D"},              /*610X解码器*/
    {30,"IPCAM"},                /*IP 摄像机*/
    {31,"MEGA_IPCAM"},                /*高清IP摄像机 852F&852MF*/
    {32,"IPCAM_862MF"},                /*862MF可以接入9000设备*/
    {35,"ITCCAM"},                /*智能高清网络摄像机*/
    {36,"IVS_IPCAM"},          /*智能分析高清网络摄像机*/
    {38,"ZOOMCAM"},            /*一体机*/
    {40,"IPDOME"},              /*IP 标清球机*/
    {41,"IPDOME_MEGA200"},              /*IP 200万高清球机*/
    {42,"IPDOME_MEGA130"},              /*IP 130万高清球机*/
    {43,"IPDOME_AI"},              /*IP 高清智能快球*/ 
    {44,"TII_IPCAM"},          /*红外热成像摄像机*/
    {50,"IPMOD"},                /*IP 模块*/
    {59,"DS64XXHD_T"},                //64-T高清解码器
    {60,"IDS6501_HF_P"},    // 6501 车牌
    {61,"IDS6101_HF_A"},              //智能ATM
    {62,"IDS6002_HF_B"},          //双机跟踪：DS6002-HF/B
    {63,"IDS6101_HF_B"},          //行为分析：DS6101-HF/B
    {64,"IDS52XX"},          //智能分析仪IVMS
    {65,"IDS90XX"},                // 9000智能
    {67,"IDS8104_AHL_S_H"},              // 海康人脸识别 ATM
    {68,"IDS91XX"},                // 9100智能
    {69,"IIP_CAM_B"},  //智能行为IP摄像机
    {70,"IIP_CAM_F"},  //智能人脸IP摄像机
    {71,"DS71XX_H"},                /* DS71XXH_S */
    {72,"DS72XX_H_S"},                /* DS72XXH_S */
    {73,"DS73XX_H_S"},                /* DS73XXH_S */
    {74,"DS72XX_HF_S"},              //DS72XX_HF_S
    {75,"DS73XX_HFI_S"},              //DS73XX_HFI_S
    {75,"DS73XX_HF_S"},              //DS73XX_HF_S
    {76,"DS76XX_H_S"},                /* DVR,e.g. DS7604_HI_S */
    {77,"DS76XX_N_S"},                /* NVR,e.g. DS7604_NI_S */
    {81,"DS81XX_HS_S"},                /* DS81XX_HS_S */
    {82,"DS81XX_HL_S"},                /* DS81XX_HL_S */
    {83,"DS81XX_HC_S"},                /* DS81XX_HC_S */
    {84,"DS81XX_HD_S"},                /* DS81XX_HD_S */
    {85,"DS81XX_HE_S"},                /* DS81XX_HE_S */
    {86,"DS81XX_HF_S"},                /* DS81XX_HF_S */
    {87,"DS81XX_AH_S"},                /* DS81XX_AH_S */
    {88,"DS81XX_AHF_S"},                /* DS81XX_AHF_S */
    {90,"DS90XX_HF_S"},              /*DS90XX_HF_S*/
    {91,"DS91XX_HF_S"},              /*DS91XX_HF_S*/
    {92,"DS91XX_HD_S"},              /*91XXHD-S(MD)*/
    {93,"IDS90XX_A"},                // 9000智能 ATM
    {94,"IDS91XX_A"},                // 9100智能 ATM
    {95,"DS95XX_N_S"},              /*DS95XX_N_S NVR 不带任何输出*/
    {96,"DS96XX_N_SH"},              /*DS96XX_N_SH NVR*/
    {97,"DS90XX_HF_SH"},              /*DS90XX_HF_SH */   
    {98,"DS91XX_HF_SH"},              /*DS91XX_HF_SH */
    {100,"DS_B10_XY"},             /*视频综合平台设备型号(X:编码板片数，Y:解码板片数)*/
    {101,"DS_6504HF_B10"},             /*视频综合平台内部编码器*/
    {102,"DS_6504D_B10"},             /*视频综合平台内部解码器*/
    {103,"DS_1832_B10"},             /*视频综合平台内部码分器*/
    {104,"DS_6401HFH_B10"},             /*视频综合平台内部光纤板*/
    {105,"DS_65XXHC"},                //65XXHC DVS
    {106,"DS_65XXHC_S"},                //65XXHC-SATA DVS
    {107,"DS_65XXHF"},                //65XXHF DVS
    {108,"DS_65XXHF_S"},                //65XXHF-SATA DVS
    {109,"DS_6500HF_B"},             //65 rack DVS
    {110,"IVMS_6200_C"},            // iVMS-6200(/C)  
    {111,"IVMS_6200_B"},             // iVMS-6200(/B)
    {112,"DS_72XXHV_ST"},                //72XXHV_ST15 DVR
    {113,"DS_72XXHV_ST"},                //72XXHV_ST20 DVR,这两个设备显示一个名字
    {114,"IVMS_6200_T"},             // IVMS-6200(/T)
    {115,"IVMS_6200_BP"},             // IVMS-6200(/BP)
    {116,"DS_81XXHC_ST"},                //DS_81XXHC_ST
    {117,"DS_81XXHS_ST"},                //DS_81XXHS_ST
    {118,"DS_81XXAH_ST"},                //DS_81XXAH_ST
    {119,"DS_81XXAHF_ST"},                //DS_81XXAHF_ST
    {120,"DS_66XXDVS"},                //66XX DVS
    {121,"DS_1964_B10"},             /*视频综合平台内部报警器*/
    {122,"DS_B10N04_IN"},             /*视频综合平台内部级联输入*/
    {123,"DS_B10N04_OUT"},             /*视频综合平台内部级联输出*/
    {124,"DS_B10N04_INTEL"},             /*视频综合平台内部智能*/
    {125,"DS_6408HFH_B10E_RM"},             //V6高清
    {126,"DS_B10N64F1_RTM"},             //V6级联不带DSP
    {127,"DS_B10N64F1D_RTM"},             //V6级联带DSP
    {128,"DS_B10_SDS"},             //视频综合平台子域控制器
    {129,"DS_B10_DS"},             //视频综合平台域控制器
    {130,"DS_6401HFH_B10V"},             //VGA高清编码器
    {131,"DS_6504D_B10B"},             /*视频综合平台内部标清解码器*/
    {132,"DS_6504D_B10H"},             /*视频综合平台内部高清解码器*/
    {133,"DS_6504D_B10V"},             /*视频综合平台内部VGA解码器*/
    {134,"DS_6408HFH_B10S"},             //视频综合平台SDI子板 
    {135,"DS_18XX_N"},             /* 矩阵接入网关*/ 
    //注：121～140预留给视频综合平台系列产品使用,141~160留给码分设备和报警设备类用
    {141,"DS_18XX_PTZ"},                /*网络码分类产品*/
    {142,"DS_19AXX"},                /*通用报警主机类产品*/
    {143,"DS_19BXX"},                /*家用报警主机*/
    {144,"DS_19CXX"},                /*自助银行报警主机*/
    {145,"DS_19DXX"},                /*动环监控报警主机*/    
    {146,"DS_19XX"},             /*1900系列报警主机*/ 
    {147,"DS_19SXX"},                /*视频报警主机*/
    {148, "DS_1HXX"},            /*CS类产品*/ //防护舱
    /**********************设备类型 end***********************/
    //2011-11-03 161~170分配给大屏设备使用
    {161,"DS_C10H"},           /*多屏控制器*/
    {162,"DS_C10N_BI"},            //BNC处理器
    {163,"DS_C10N_DI"},            //rbg处理器
    {164,"DS_C10N_SI"},            //码流处理器
    {165,"DS_C10N_DO"},            //显示处理器
    {166,"DS_C10N_SERVER"},        //分布式服务器
    // 171- 180  划给智能使用
    {171,"IDS_8104_AHFL_S_H"},             // 8104ATM 
    {172,"IDS_65XX_HF_A"},             // 65 ATM
    {173,"IDS90XX_HF_RH"},             // 9000 智能RH
    {174,"IDS91XX_HF_RH"},             // 9100 智能RH设备
    {175,"IDS_65XX_HF_B"},             // 65 行为分析
    {176,"IDS_65XX_HF_P"},             // 65 车牌识别
    {177,"IVMS_6200_F"},     // IVMS-6200(/F)
    {178,"IVMS_6200_A"},             //iVMS-6200(/A)
    {179,"IVMS_6200_F_S"},   //iVMS-6200(/F_S)
    {181,"DS90XX_HF_RH"},             // 9000 RH
    {182,"DS91XX_HF_RH"},             // 9100 RH设备
    {183,"DS78XX_S"},             // 78系列设备
    {185,"DS81XXHW_S"},                // 81 Resolution 960 
    {186,"DS81XXHW_ST"},             // DS81XXHW_ST
    {187,"DS91XXHW_ST"},             // DS91XXHW_ST
    {188,"DS91XX_ST"},             // DS91XX_ST
    {189,"DS81XX_ST"},             // DS81XX_ST
    {190,"DS81XXH_ST"},             // DS81XXHDI_ST,DS81XXHE_ST ky2012
    {191,"DS73XXH_ST"},             // DS73XXHI_ST ky2012
    {192,"DS81XX_SH"},             // 审讯81SH,81SHF
    {193,"DS81XX_SN"},             // 审讯81SNL
    {194,"DS96XXN_ST"},             //NVR:DS96xxN_ST
    {195,"DS86XXN_ST"},             //NVR:DS86xxN_ST
    {196,"DS80XXHF_ST"},             //DVR:DS80xxHF_ST
    {197,"DS90XXHF_ST"},             //DVR:DS90xxHF_ST
    {198,"DS76XXN_ST"},             //NVR:DS76xxN_ST
    {199,"DS_9664N_RX"},         //嵌入式NVR(64路IP通道，无模拟通道)，最后一个X：T/H    
    {200,"ENCODER_SERVER"},            // 编码卡服务器
    {201,"DECODER_SERVER"},         // 解码卡服务器
    {202,"PCNVR_SERVER"},         // PCNVR存储服务器
    {203,"CVR_SERVER"},         // 邦诺CVR，他给自己定的类型为DVR_S-1
    {204,"DS_91XXHFH_ST"},         // 91系列HD-SDI高清DVR
    {205,"DS_66XXHFH"},         // 66高清编码器    
    {210,"TRAFFIC_TS_SERVER"},         //终端服务器
    {211,"TRAFFIC_VAR"},         //视频分析记录仪
    {212,"IPCALL"},              //IP可视对讲分机

    //2012-03-21 类型值255以后的设备不要添加至此数组中，该数组的作用是为了使扩展协议能获取类型值255以下设备的名称
    //{2001, "DS90XXHW_ST"}        // DS90XXHW_ST混合DVR
};

typedef struct tagNET_EHOME_DEVICE_INFO
{
    DWORD        dwSize;                //结构体大小
    DWORD        dwChannelNumber;     //模拟视频通道个数
    DWORD        dwChannelAmount;    //总视频通道数（模拟+IP）
    DWORD        dwDevType;            //设备类型，1-DVR，3-DVS，30-IPC，40-IPDOME，其他值参考海康NetSdk设备类型号定义值
    DWORD        dwDiskNumber;        //设备当前硬盘数
    BYTE        sSerialNumber[MAX_SERIALNO_LEN];        //设备序列号
    DWORD        dwAlarmInPortNum;    //模拟通道报警输入个数
    DWORD        dwAlarmInAmount;    //设备总报警输入个数
    DWORD        dwAlarmOutPortNum;    //模拟通道报警输出个数
    DWORD        dwAlarmOutAmount;    //设备总报警输出个数
    DWORD        dwStartChannel;        //视频起始通道号
    DWORD        dwAudioChanNum;    //语音对讲通道个数
    DWORD        dwMaxDigitChannelNum;    //设备支持的最大数字通道路数
    DWORD        dwAudioEncType;        //语音对讲音频格式，0- OggVorbis，1-G711U，2-G711A，3-G726，4-AAC，5-MP2L2,6-PCM
    BYTE        sSIMCardSN[MAX_SERIALNO_LEN];    //车载设备扩展：SIM卡序列号
    BYTE        sSIMCardPhoneNum[MAX_PHOMENUM_LEN];    //车载扩展：SIM卡手机号码
    DWORD        dwSupportZeroChan;    // SupportZeroChan:支持零通道的个数：0-不支持，1-支持1路，2-支持2路，以此类推
    DWORD        dwStartZeroChan;        //零通道起始编号，默认10000
    DWORD        dwSmartType;            //智能类型，0-smart，1-专业智能；默认0-smart
    BYTE        byRes[160];            //保留
}NET_EHOME_DEVICE_INFO,*LPNET_EHOME_DEVICE_INFO;

#define    MAX_VERSION_LEN            32  //版本描述长度
typedef struct tagNET_EHOME_VERSION_INFO
{
    DWORD        dwSize;                //结构体大小
    BYTE        sSoftwareVersion[MAX_VERSION_LEN];    //主控版本
    BYTE        sDSPSoftwareVersion[MAX_VERSION_LEN];//编码版本
    BYTE        sPanelVersion[MAX_VERSION_LEN];    //面板版本
    BYTE        sHardwareVersion[MAX_VERSION_LEN];    //硬件版本
    BYTE        byRes[124];
}NET_EHOME_VERSION_INFO,*LPNET_EHOME_VERSION_INFO;

typedef struct tagNET_EHOME_DEVICE_CFG
{
    DWORD        dwSize;                //结构体大小
    BYTE        sServerName[MAX_DEVICE_NAME_LEN];    //设备名称
    DWORD        dwServerID;            //设备号（遥控器编号:0~255）
    DWORD        dwRecycleRecord;        //是否循环录像，0代表不循环，1代表循环
    DWORD        dwServerType;        //设备类型，同DevType，由于公司设备型号有几百个，本协议中只使用通用型号：1-DVR，3-DVS，30-IPC，40-IPDOME
    DWORD        dwChannelNum;        //通道个数，包括模拟+IP通道（只读）
    DWORD        dwHardDiskNum;        //硬盘个数（只读）
    DWORD        dwAlarmInNum;        //报警输入个数（模拟通道）（只读）
    DWORD        dwAlarmOutNum;        //报警输出个数（模拟通道）（只读）
    DWORD        dwRS232Num;        //232串口个数（只读）
    DWORD        dwRS485Num;        //485串口个数（只读）
    DWORD        dwNetworkPortNum;    //网络口个数（只读）
    DWORD        dwAuxoutNum;        //辅口个数（只读）
    DWORD        dwAudioNum;        //语音口个数（只读）
    BYTE        sSerialNumber[MAX_SERIALNO_LEN];    //设备序列号（只读）
    DWORD        dwMajorScale;        //主口缩放：0代表不缩放；1代表缩放
    DWORD        dwMinorScale;        //辅口缩放：0代表不缩放；1代表缩放
    BYTE        byRes[292];            //保留
}NET_EHOME_DEVICE_CFG,*LPNET_EHOME_DEVICE_CFG;

#define    NET_EHOME_GET_DEVICE_INFO        1    //获取设备信息
#define    NET_EHOME_GET_VERSION_INFO        2    //获取版本信息
#define    NET_EHOME_GET_DEVICE_CFG        3    //获取设备基本参数
#define    NET_EHOME_SET_DEVICE_CFG        4    //设置设备基本参数

#define NET_EHOME_GET_GPS_CFG 20 //获取GPS参数
#define NET_EHOME_SET_GPS_CFG 21 //设置GPS参数
#define NET_EHOME_GET_PIC_CFG 22 //获取OSD显示参数
#define NET_EHOME_SET_PIC_CFG 23 //设置OSD显示参数
#define NET_EHOME_GET_WIRELESSINFO_CFG 24 //获取无线网络信息参数
#define NET_EHOME_SET_WIRELESSINFO_CFG 25 //设置无线网络信息参数

#define MAX_EHOME_PROTOCOL_LEN  1500

typedef struct tagNET_EHOME_REMOTE_CTRL_PARAM
{
    DWORD dwSize;
    void *lpCondBuffer;        //条件参数缓冲区
    DWORD  dwCondBufferSize;    //条件参数缓冲区长度
    void *lpInbuffer;          //控制参数缓冲区
    DWORD  dwInBufferSize;      //控制参数缓冲区长度
    BYTE   byRes[32];
}NET_EHOME_REMOTE_CTRL_PARAM, *LPNET_EHOME_REMOTE_CTRL_PARAM;

//GPS参数
typedef struct tagNET_EHOME_GPS_CFG
{
    DWORD        dwSize;
    DWORD        dwTransInterval;    //GPS上传时间，单位：秒
    DWORD        dwMaxSpeed;        //超速限速值，单位：厘米/小时
    DWORD        dwMinSpeed;        //低速限速值，单位：厘米/小时
    BYTE         bEnable;           //使能,1-不启用，2-启用
    BYTE            byRes[63];                //保留
}NET_EHOME_GPS_CFG, *LPNET_EHOME_GPS_CFG;

//OSD显示参数
typedef struct tagNET_EHOME_PIC_CFG
{
    DWORD        dwSize;
    BYTE            byChannelName[NAME_LEN];        //通道名称（GB2312编码， V30以后版本使用UTF-8编码），长度不超过31字节
    BOOL            bIsShowChanName;                            //是否显示通道名称：0：不显示；1：显示
    WORD            wChanNameXPos;                            //通道名称显示X坐标，按照704x576来配置
    WORD            wChanNameYPos;                            //通道名称显示Y坐标，按照704x576来配置，坐标值为16的倍数
    BOOL            bIsShowOSD;                                    //是否显示OSD：0：不显示；1：显示
    WORD            wOSDXPos;                                        //OSD显示X坐标
    WORD            wOSDYPos;                                        //OSD显示Y坐标
    BYTE            byOSDType;                                        /**OSD类型 0:XXXX-XX-XX(年月日) 1:XX-XX-XXXX(月日年) 2:XXXX年XX月XX日
                                                                    3:XX月XX日XXXX年 4:XX-XX-XXXX(日月年) 5:XX日XX月XXXX年
                                                                    */
    BYTE            byOSDAtrib;                                    //OSD属性 0：不显示OSD 1：透明，闪烁 2：透明，不闪烁 3：闪烁，不透明 4：不透明，不闪烁
    BYTE            byRes1[2];                                            //保留1
    BOOL            bIsShowWeek;                                    //是否显示星期：0：不显示；1：显示
    BYTE            byRes2[64];                                        //保留2
}NET_EHOME_PIC_CFG, *LPNET_EHOME_PIC_CFG;

typedef struct tagNET_EHOME_WIRELESS_INFO_CFG
{
    DWORD        dwSize;
    DWORD        dwInfoTransInterval;   //上传间隔,单位：秒
    BYTE         byEnable; //使能，0-不启用 1-启用
    BYTE         byRes[47];
}NET_EHOME_WIRELESS_INFO_CFG, *LPNET_EHOME_WIRELESS_INFO_CFG;

typedef struct tagNET_CMS_ISAPI_CONFIG
{
    DWORD    dwSize;
    void     *lpRequestUrl;
    DWORD    dwRequestUrlLen;
    void     *lpInBuffer;
    DWORD    dwInBufferSize;
    void     *lpOutBuffer;
    DWORD    dwOutBufferSize;
    DWORD    dwReturnedSize;
    DWORD    dwRecvTimeOut;
    BYTE     byRes[32];
}NET_CMS_ISAPI_CONFIG, *LPNET_CMS_ISAPI_CONFIG;

NET_DVR_API BOOL CALLBACK NET_ECMS_GetDevConfig(LONG lUserID, DWORD dwCommand, LPNET_EHOME_CONFIG lpConfig, DWORD dwConfigSize);

NET_DVR_API BOOL CALLBACK NET_ECMS_SetDevConfig(LONG lUserID, DWORD dwCommand, LPNET_EHOME_CONFIG lpConfig, DWORD dwConfigSize);

NET_DVR_API BOOL CALLBACK NET_ECMS_XMLConfig(LONG lUserID, LPNET_EHOME_XML_CFG pXmlCfg, DWORD dwConfigSize);

NET_DVR_API BOOL CALLBACK NET_ECMS_RemoteControl(LONG lUserID, DWORD dwCommand, LPNET_EHOME_REMOTE_CTRL_PARAM lpCtrlParam);

NET_DVR_API BOOL CALLBACK NET_ECMS_ISAPI_Config(LONG lUserID, NET_CMS_ISAPI_CONFIG *lpParam);

//预览请求
typedef struct tagNET_EHOME_PREVIEWINFO_IN
{
    int iChannel;                        //通道号 
    DWORD dwStreamType;                    // 码流类型，0-主码流，1-子码流, 2-第三码流
    DWORD dwLinkMode;                        // 0：TCP方式,1：UDP方式,2: HRUDP方式
    NET_EHOME_IPADDRESS struStreamSever;     //流媒体地址
}NET_EHOME_PREVIEWINFO_IN, *LPNET_EHOME_PREVIEWINFO_IN;

typedef struct tagNET_EHOME_PREVIEWINFO_IN_V11
{
    int iChannel;
    DWORD dwStreamType;
    DWORD dwLinkMode;
    NET_EHOME_IPADDRESS struStreamSever;
    BYTE  byDelayPreview;
    BYTE  byRes[31];
}NET_EHOME_PREVIEWINFO_IN_V11, *LPNET_EHOME_PREVIEWINFO_IN_V11;

typedef struct tagNET_EHOME_PREVIEWINFO_OUT
{
    LONG  lSessionID; 
    BYTE byRes[128];
}NET_EHOME_PREVIEWINFO_OUT, *LPNET_EHOME_PREVIEWINFO_OUT;

typedef struct tagNET_EHOME_PUSHSTREAM_IN
{
    DWORD dwSize;
    LONG  lSessionID; 
    BYTE  byRes[128];
}NET_EHOME_PUSHSTREAM_IN, *LPNET_EHOME_PUSHSTREAM_IN;

typedef struct tagNET_EHOME_PUSHSTREAM_INFO_OUT
{
    DWORD dwSize;
    BYTE  byRes[128];
}NET_EHOME_PUSHSTREAM_OUT, *LPNET_EHOME_PUSHSTREAM_OUT;

NET_DVR_API BOOL CALLBACK NET_ECMS_StartGetRealStream(LONG lUserID, LPNET_EHOME_PREVIEWINFO_IN pPreviewInfoIn, LPNET_EHOME_PREVIEWINFO_OUT pPreviewInfoOut ); //lUserID由SDK分配的用户ID，由设备注册回调时fDeviceRegisterCallBack返回
NET_DVR_API BOOL CALLBACK NET_ECMS_StartGetRealStreamV11(LONG lUserID, LPNET_EHOME_PREVIEWINFO_IN_V11 pPreviewInfoIn, LPNET_EHOME_PREVIEWINFO_OUT pPreviewInfoOut);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopGetRealStream(LONG lUserID, LONG lSessionID);
NET_DVR_API BOOL CALLBACK NET_ECMS_StartPushRealStream(LONG lUserID, LPNET_EHOME_PUSHSTREAM_IN pPushInfoIn, LPNET_EHOME_PUSHSTREAM_OUT pPushInfoOut);

//查询接口
#define MAX_FILE_NAME_LEN  100
#define LEN_32             32

typedef enum tagSEARCH_TYPE
{
    ENUM_SEARCH_TYPE_ERR = -1,
    ENUM_SEARCH_RECORD_FILE = 0,    //查找录像文件
    ENUM_SEARCH_PICTURE_FILE  = 1,  //查找图片文件
    ENUM_SEARCH_FLOW_INFO = 2,      //流量查询
    ENUM_SEARCH_DEV_LOG = 3,        //设备日志查询
    ENUM_SEARCH_ALARM_HOST_LOG = 4, //报警主机日志查询
}SEARCH_TYPE_ENUM;

typedef enum
{
    ENUM_GET_NEXT_STATUS_SUCCESS = 1000,    //成功读取到一条数据，处理完本次数据后需要再次调用FindNext获取下一条数据
    ENUM_GET_NETX_STATUS_NO_FILE,           //没有找到一条数据
    ENUM_GET_NETX_STATUS_NEED_WAIT,         //数据还未就绪，需等待，继续调用FindNext函数
    ENUM_GET_NEXT_STATUS_FINISH,            //数据全部取完
    ENUM_GET_NEXT_STATUS_FAILED,            //出现异常
    ENUM_GET_NEXT_STATUS_NOT_SUPPORT        //设备不支持该操作，不支持的查询类型
}SEARCH_GET_NEXT_STATUS_ENUM;

//时间参数
typedef struct tagNET_EHOME_TIME
{
    WORD    wYear;      //年
    BYTE    byMonth;    //月
    BYTE    byDay;      //日
    BYTE    byHour;     //时
    BYTE    byMinute;   //分
    BYTE    bySecond;   //秒
    BYTE    byRes1;        
    WORD    wMSecond;   //毫秒
    BYTE    byRes2[2];
}NET_EHOME_TIME, *LPNET_EHOME_TIME;

typedef struct tagNET_EHOME_FINDCOND
{
    DWORD               dwSize;
    LONG                iChannel;           //通道号，从1开始
    DWORD               dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    NET_EHOME_TIME      struStartTime;      //开始时间
    NET_EHOME_TIME      struStopTime;       //结束时间
    SEARCH_TYPE_ENUM    enumSearchType;     //查找类型 0-录像文件查找，对应struRecordFileParam  1-图片文件查找，对应struPicFileParam 2-流量查询，对应struFlowParam
    union
    {
        BYTE            byLen[64];
        struct
        {
            DWORD       dwFileType;         /**文件类型（字节整形数）：
                                            0xff-全部类型录像（不包含图片类型） 
                                            0-定时录像
                                            1-移动报警
                                            2-报警触发
                                            3-报警|动测
                                            4-报警&动测
                                            5-命令触发
                                            6-手动录像
                                            7-震动报警
                                            8-环境报警
                                            9-智能报警（或者取证录像）
                                            10（0x0a）-PIR报警
                                            11（0x0b）-无线报警
                                            12（0x0c）-呼救报警
                                            13（0x0d）全部报警
                                            100-全部类型图片
                                            101-车牌识别图片
                                            102-稽查报警图片
                                            103-手动抓拍图片
                                            104-回放抓拍图片
                                            **/

       }struRecordFileParam;
        struct
        {
            DWORD       dwFileType;         /*255（0xff）-全部类型：
                                            0（0x00）-定时抓图
                                            1（0x01）-移动侦测抓图
                                            2（0x02）-报警抓图
                                            3（0x03）-报警|移动侦测抓图
                                            4（0x04）-报警&移动侦测抓图
                                            5（0x05）-命令触发抓图
                                            6（0x06）-手动抓图
                                            7（0x07）-震动报警抓图
                                            8（0x08）-环境报警触发抓图
                                            9（0x09）-智能报警图片
                                            10（0x0a）-PIR报警图片
                                            11（0x0b）-无线报警图片
                                            12（0x0c）-呼救报警图片
                                            13（0x0d）-人脸侦测图片
                                            14（0x0e）-越界侦测图片
                                            15（0x0f）-入侵区域侦测图片
                                            16（0x10）-场景变更侦测图片
                                            17（0x11）-设备本地回放时截图
                                            18（0x12）-智能侦测图片
                                            19（0x13）-进入区域侦测图片
                                            20（0x14）-离开区域侦测图片
                                            21（0x15）-徘徊侦测图片
                                            22（0x16）-人员聚集侦测图片
                                            23（0x17）-快速运动侦测图片
                                            24（0x18）-停车侦测图片
                                            25（0x19）-物品遗留侦测图片
                                            26（0x1a）-物品拿取侦测图片
                                            27（0x1b）-车牌侦测图片
                                            28（0x1c）-客户端上传图片
                                            */
        }struPicFileParam;
        struct
        {
            BYTE        bySearchMode;       //查询模式，1-按year查询，2-按month查询，3-按day查询
        }struFlowParam;
    }unionSearchParam;
	BYTE                byStartIndex;       //查询起始位置，兼容老版本，新版本中不建议使用
	BYTE                byRes1[1];
	DWORD               dwStartIndexEx;     //扩展后的查询起始位置， 增加对记录条数过大的支持
	BYTE                byRes[122];
}NET_EHOME_FINDCOND, *LPNET_EHOME_FINDCOND;

//录像文件查找条件
typedef struct tagNET_EHOME_REC_FILE_COND
{
    DWORD           dwChannel;          //通道号，从1开始
    DWORD           dwRecType;          /*0xff-全部类型录像（不包含图片类型）
                                        0-定时录像
                                        1-移动报警
                                        2-报警触发
                                        3-报警|动测
                                        4-报警&动测
                                        5-命令触发
                                        6-手动录像
                                        7-震动报警
                                        8-环境报警
                                        9-智能报警（或者取证录像）
                                        10（0x0a）-PIR报警
                                        11（0x0b）-无线报警
                                        12（0x0c）-呼救报警
                                        13（0x0d）全部报警
                                        100-全部类型图片
                                        101-车牌识别图片
                                        102-稽查报警图片
                                        103-手动抓拍图片
                                        104-回放抓拍图片
                                        */
    NET_EHOME_TIME  struStartTime;      //开始时间
    NET_EHOME_TIME  struStopTime;       //结束时间
    DWORD           dwStartIndex;       //查询起始位置
    DWORD           dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    BYTE            byLocalOrUTC;        //0-struStartTime和struStopTime中，表示的是设备本地时间，即设备OSD时间  1-struStartTime和struStopTime中，表示的是UTC时间
    BYTE            byRes[63];
}NET_EHOME_REC_FILE_COND, *LPNET_EHOME_REC_FILE_COND;

//图片文件查找条件
typedef struct tagNET_EHOME_PIC_FILE_COND
{
    DWORD           dwChannel;          //通道号，从1开始
    DWORD           dwPicType;          /*255（0xff）-全部类型
                                        0（0x00）-定时抓图
                                        1（0x01）-移动侦测抓图
                                        2（0x02）-报警抓图
                                        3（0x03）-报警|移动侦测抓图
                                        4（0x04）-报警&移动侦测抓图
                                        5（0x05）-命令触发抓图
                                        6（0x06）-手动抓图
                                        7（0x07）-震动报警抓图
                                        8（0x08）-环境报警触发抓图
                                        9（0x09）-智能报警图片
                                        10（0x0a）-PIR报警图片
                                        11（0x0b）-无线报警图片
                                        12（0x0c）-呼救报警图片
                                        13（0x0d）-人脸侦测图片
                                        14（0x0e）-越界侦测图片
                                        15（0x0f）-入侵区域侦测图片
                                        16（0x10）-场景变更侦测图片
                                        17（0x11）-设备本地回放时截图
                                        18（0x12）-智能侦测图片
                                        19（0x13）-进入区域侦测图片
                                        20（0x14）-离开区域侦测图片
                                        21（0x15）-徘徊侦测图片
                                        22（0x16）-人员聚集侦测图片
                                        23（0x17）-快速运动侦测图片
                                        24（0x18）-停车侦测图片
                                        25（0x19）-物品遗留侦测图片
                                        26（0x1a）-物品拿取侦测图片
                                        27（0x1b）-车牌侦测图片
                                        28（0x1c）-客户端上传图片
                                        */
    NET_EHOME_TIME  struStartTime;      //开始时间
    NET_EHOME_TIME  struStopTime;       //结束时间
    DWORD           dwStartIndex;       //查询起始位置
    DWORD           dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    BYTE            byLocalOrUTC;        //0-struStartTime和struStopTime中，表示的是设备本地时间，即设备OSD时间  1-struStartTime和struStopTime中，表示的是UTC时间
    BYTE            byRes[63];
}NET_EHOME_PIC_FILE_COND, *LPNET_EHOME_PIC_FILE_COND;

//流量查询条件
typedef struct tagNET_EHOME_FLOW_COND
{
    BYTE            bySearchMode;       //查询模式，1-按year查询，2-按month查询，3-按day查询
    BYTE            byRes[3];
    NET_EHOME_TIME  struStartTime;      //开始时间
    NET_EHOME_TIME  struStopTime;       //结束时间
    DWORD           dwStartIndex;       //查询起始位置
    DWORD           dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    BYTE            byLocalOrUTC;        //0-struStartTime和struStopTime中，表示的是设备本地时间，即设备OSD时间  1-struStartTime和struStopTime中，表示的是UTC时间
    BYTE            byRes1[63];
}NET_EHOME_FLOW_COND, *LPNET_EHOME_FLOW_COND;

//设备日志查询条件
typedef struct tagNET_EHOME_DEV_LOG_COND
{
    DWORD           dwMajorType;        //日志主类型，1-报警，2-异常，3-操作，0xffff-全部
    DWORD           dwMinorType;        //日志次类型，与设备基线代码定义相同
    NET_EHOME_TIME  struStartTime;      //开始时间
    NET_EHOME_TIME  struStopTime;       //结束时间
    DWORD           dwStartIndex;       //查询起始位置
    DWORD           dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    BYTE            byLocalOrUTC;        //0-struStartTime和struStopTime中，表示的是设备本地时间，即设备OSD时间  1-struStartTime和struStopTime中，表示的是UTC时间
    BYTE            byRes[63];
}NET_EHOME_DEV_LOG_COND, *LPNET_EHOME_DEV_LOG_COND;

//报警主机日志查询条件
typedef struct tagNET_EHOME_ALARM_HOST_LOG_COND
{
    DWORD           dwMajorType;        //日志主类型，1-报警，2-异常，3-操作，4-事件，0xffff-全部
    DWORD           dwMinorType;        //日志次类型，与设备基线代码定义相同
    NET_EHOME_TIME  struStartTime;      //开始时间
    NET_EHOME_TIME  struStopTime;       //结束时间
    DWORD           dwStartIndex;       //查询起始位置
    DWORD           dwMaxFileCountPer;  //单次搜索最大文件个数，最大文件个数，需要确定实际网络环境，建议最大个数为8
    BYTE            byLocalOrUTC;        //0-struStartTime和struStopTime中，表示的是设备本地时间，即设备OSD时间  1-struStartTime和struStopTime中，表示的是UTC时间
    BYTE            byRes1[63];
}NET_EHOME_ALARM_HOST_LOG_COND, *LPNET_EHOME_ALARM_HOST_LOG_COND;

//老版的录像文件信息
typedef struct tagNET_EHOME_FINDDATA
{
    DWORD           dwSize;
    char            szFileName[MAX_FILE_NAME_LEN];  //文件名
    NET_EHOME_TIME  struStartTime;                  //文件的开始时间
    NET_EHOME_TIME  struStopTime;                   //文件的结束时间
    DWORD           dwFileSize;                     //文件的大小
    DWORD           dwFileMainType;                 //录像文件主类型
    DWORD           dwFileSubType;                  //录像文件子类型
    DWORD           dwFileIndex;                    //录像文件索引
    BYTE            byRes[128];
}NET_EHOME_FINDDATA, *LPNET_EHOME_FINDDATA;

//录像文件信息
typedef struct tagNET_EHOME_REC_FILE
{
    DWORD           dwSize;
    char            sFileName[MAX_FILE_NAME_LEN];   //文件名
    NET_EHOME_TIME  struStartTime;                  //文件的开始时间
    NET_EHOME_TIME  struStopTime;                   //文件的结束时间
    DWORD           dwFileSize;                     //文件的大小
    DWORD           dwFileMainType;                 //录像文件主类型
    DWORD           dwFileSubType;                  //录像文件子类型
    DWORD           dwFileIndex;                    //录像文件索引
    BYTE            byTimeDiffH;                    //struStartTime、struStopTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE            byTimeDiffM;                    //struStartTime、struStopTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE            byRes[126];
}NET_EHOME_REC_FILE, *LPNET_EHOME_REC_FILE;

//图片文件信息
typedef struct tagNET_EHOME_PIC_FILE
{
    DWORD           dwSize;
    char            sFileName[MAX_FILE_NAME_LEN];   //文件名
    NET_EHOME_TIME  struPicTime;                    //图片生成时间
    DWORD           dwFileSize;                     //文件的大小
    DWORD           dwFileMainType;                 //图片文件主类型
    DWORD           dwFileIndex;                    //图片文件索引
    BYTE            byTimeDiffH;                    //struPicTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE            byTimeDiffM;                    //struPicTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE            byRes[126];
}NET_EHOME_PIC_FILE, *LPNET_EHOME_PIC_FILE;

//流量信息
typedef struct tagNET_EHOME_FLOW_INFO
{
    DWORD   dwSize;
    DWORD   dwFlowValue;    //流量值
    DWORD   dwFlowIndex;    //流量索引
    BYTE    byRes[128];
}NET_EHOME_FLOW_INFO, *LPNET_EHOME_FLOW_INFO;

//设备日志信息
#define MAX_LOG_INFO_LEN    8*1024   //日志附加信息长度
typedef struct tagNET_EHOME_DEV_LOG
{
    NET_EHOME_TIME  struLogTime;                    //日志时间
    DWORD           dwMajorType;                    //日志主类型，1-报警，2-异常，3-操作
    DWORD           dwMinorType;                    //日志次类型，与设备基线代码定义相同
    DWORD           dwParamType;                    //次类型参数类型，大部分表示防区号，如果没有则值为0
    char            sLocalUser[NAME_LEN];           //本地用户
    char            sRemoteUser[NAME_LEN];          //远程用户
    char            sIPAddress[128];                //远端主机IP地址
    DWORD           dwChannelNo;                    //通道号
    DWORD           dwHardDiskNo;                   //硬盘号
    DWORD           dwAlarmInputChanNo;             //报警输入通道号
    DWORD           dwAlarmOutputChanNo;            //报警输出通道号
    char            sLogContext[MAX_LOG_INFO_LEN];  //日志内容
    BYTE            byTimeDiffH;                    //struLogTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE            byTimeDiffM;                    //struLogTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE            byRes[62];
}NET_EHOME_DEV_LOG, *LPNET_EHOME_DEV_LOG;

//报警主机日志信息
typedef struct tagNET_EHOME_ALARM_HOST_LOG
{
    NET_EHOME_TIME  struLogTime;                    //日志时间
    DWORD           dwMajorType;                    //日志主类型，1-报警，2-异常，3-操作，4-事件
    DWORD           dwMinorType;                    //日志次类型，与设备基线代码定义相同
    DWORD           dwParamType;                    //次类型参数类型，大部分表示防区号，如果没有则值为0
    char            sUserName[NAME_LEN];            //用户名
    char            sIPAddress[128];                //远端主机IP地址
    char            sLogContext[MAX_LOG_INFO_LEN];  //日志内容
    BYTE            byTimeDiffH;                    //struLogTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE            byTimeDiffM;                    //struLogTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE            byRes[62];
}NET_EHOME_ALARM_HOST_LOG, *LPNET_EHOME_ALARM_HOST_LOG;

NET_DVR_API LONG CALLBACK NET_ECMS_StartFindFile(LONG lUserID, LPNET_EHOME_FINDCOND pFindCond); //仅支持录像文件、图片文件和流量查询
NET_DVR_API LONG CALLBACK NET_ECMS_FindNextFile(LONG lHandle, LPNET_EHOME_FINDDATA pFindData); //仅支持录像文件查询

NET_DVR_API LONG CALLBACK NET_ECMS_StartFindFile_V11(LONG lUserID, LONG lSearchType, LPVOID pFindCond, DWORD dwCondSize);
NET_DVR_API LONG CALLBACK NET_ECMS_FindNextFile_V11(LONG lHandle, LPVOID pFindData, DWORD dwDataSize);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopFindFile(LONG lHandle);


typedef struct tagNET_EHOME_PLAYBACK_INFO_IN
{
    DWORD       dwSize;
    DWORD       dwChannel;                    //回放的通道号
    BYTE        byPlayBackMode;               //回放下载模式 0－按名字 1－按时间
    BYTE        byStreamPackage;               //回放码流类型，设备端发出的码流格式 0－PS（默认） 1－RTP
    BYTE        byRes[2];
    union
    {
        BYTE    byLen[512];
        struct
        {
            char   szFileName[MAX_FILE_NAME_LEN];          //回放的文件名
            DWORD  dwSeekType;                             //0-按字节长度计算偏移量  1-按时间（秒数）计算偏移量
            DWORD  dwFileOffset;                           //文件偏移量，从哪个位置开始下载，如果dwSeekType为0，偏移则以字节计算，为1则以秒数计算
            DWORD  dwFileSpan;                             //下载的文件大小，为0时，表示下载直到该文件结束为止，如果dwSeekType为0，大小则以字节计算，为1则以秒数计算
        }struPlayBackbyName;
        struct 
        {
            NET_EHOME_TIME  struStartTime;            // 按时间回放的开始时间
            NET_EHOME_TIME  struStopTime;            // 按时间回放的结束时间
            BYTE    byLocalOrUTC;                     //0-设备本地时间，即设备OSD时间  1-UTC时间
            BYTE    byDuplicateSegment;                //byLocalOrUTC为1时无效 0-重复时间段的前段 1-重复时间段后端
        }struPlayBackbyTime;
    }unionPlayBackMode;
    NET_EHOME_IPADDRESS struStreamSever;     //流媒体地址
}NET_EHOME_PLAYBACK_INFO_IN, *LPNET_EHOME_PLAYBACK_INFO_IN;

typedef struct tagNET_EHOME_PLAYBACK_INFO_OUT
{
    LONG   lSessionID;     //目前协议不支持，返回-1
    BYTE   byRes[128];
}NET_EHOME_PLAYBACK_INFO_OUT, *LPNET_EHOME_PLAYBACK_INFO_OUT;

typedef struct tagNET_EHOME_PUSHPLAYBACK_IN
{
    DWORD dwSize;
    LONG lSessionID; 
    BYTE  byRes[128];
} NET_EHOME_PUSHPLAYBACK_IN, *LPNET_EHOME_PUSHPLAYBACK_IN;

typedef struct tagNET_EHOME_PUSHPLAYBACK_OUT
{
    DWORD dwSize;
    BYTE  byRes[128];
} NET_EHOME_PUSHPLAYBACK_OUT, *LPNET_EHOME_PUSHPLAYBACK_OUT;

NET_DVR_API BOOL CALLBACK NET_ECMS_StartPlayBack(LONG lUserID, LPNET_EHOME_PLAYBACK_INFO_IN pPlayBackInfoIn, LPNET_EHOME_PLAYBACK_INFO_OUT pPlayBackInfoOut);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopPlayBack(LONG lUserID, LONG lSessionID);
NET_DVR_API BOOL CALLBACK NET_ECMS_StartPushPlayBack(LONG lUserID, LPNET_EHOME_PUSHPLAYBACK_IN pPushInfoIn, LPNET_EHOME_PUSHPLAYBACK_OUT pPushInfoOut);

//云台相关结构体
#define NET_EHOME_PTZ_CTRL                1000        //云镜控制
#define NET_EHOME_PRESET_CTRL        1001        //预置点操作
#define NET_EHOME_PZIN                        1002        //3D框选放大缩小
#define NET_EHOME_PTRACK                    1003        //3D点击居中

typedef enum
{
    PTZ_UP = 0,            //向上
    PTZ_DOWN,            //向下
    PTZ_LEFT,                //向左
    PTZ_RIGHT,            //向右
    PTZ_UPLEFT,            //向左上
    PTZ_DOWNLEFT,        //向左下
    PTZ_UPRIGHT,            //向右上
    PTZ_DOWNRIGHT,        //向右下
    PTZ_ZOOMIN,            //变焦－
    PTZ_ZOOMOUT,        //变焦＋
    PTZ_FOCUSNEAR,        //聚焦－
    PTZ_FOCUSFAR,        //聚焦＋
    PTZ_IRISSTARTUP,        //光圈变大
    PTZ_IRISSTOPDOWN,    //光圈变小
    PTZ_LIGHT,            //补光灯
    PTZ_WIPER,            //雨刷
    PTZ_AUTO            //自动
}EN_PTZ_CMD;

typedef struct tagNET_EHOME_PTZ_PARAM
{
    DWORD dwSize;
    BYTE  byPTZCmd;        //云台命令，参见EN_PTZ_CMD
    BYTE  byAction;        //云台动作，0-开始云台动作，1-停止云台动作
    BYTE  bySpeed;        //云台速度，0-7，数值越大速度越快
    BYTE  byRes[29];
}NET_EHOME_PTZ_PARAM, *LPNET_EHOME_PTZ_PARAM;

typedef struct tagNET_EHOME_PRESET_PARAM
{
    DWORD dwSize;
    BYTE  byPresetCmd;    //预置点控制命令，1-设置预置点，2-清除预置点，3-转到预置点
    BYTE  byRes1[3];
    DWORD dwPresetIndex;    //预置点编号
    BYTE  byRes2[32];
}NET_EHOME_PRESET_PARAM, *LPNET_EHOME_PRESET_PARAM;

typedef struct tagNET_EHOME_PZIN_PARAM
{
    DWORD dwSize;
    BYTE  byAction;    //动作，0缩小（右上->左下，右下->左上） 1放大（左上->右下，左下->右上）
    BYTE  byRes1[3];
    NET_EHOME_ZONE struArea;    //框选范围
    BYTE  byRes2[32];
}NET_EHOME_PZIN_PARAM, *LPNET_EHOME_PZIN_PARAM;

typedef struct tagNET_EHOME_POINT
{
    DWORD dwX;
    DWORD dwY;
    BYTE  byRes[4];
}NET_EHOME_POINT, *LPNET_EHOME_POINT;

typedef struct tagNET_EHOME_IPADDR
{
    char         sIpV4[16];
    char        sIpV6[128];    
}NET_EHOME_IPADDR, *LPNET_EHOME_IPADDR;

#define MACADDR_LEN                    6       //mac地址长度

typedef struct tagNET_EHOME_ETHERNET
{
    NET_EHOME_IPADDR    struDevIP;        //设备IP地址
    NET_EHOME_IPADDR    struDevIPMask;    //掩码地址
    DWORD        dwNetInterface; // 网卡类型1:10M半双工 2:10M全双工 3:100M半双工 4:100M全双工 6:1000M全双工 5:10M/100M/1000M自适应
    WORD        wDevPort;                    //设备SDK端口号：8000
    WORD        wMTU;                        // MTU参数
    BYTE        byMACAddr[MACADDR_LEN]; //mac地址
    BYTE        byRes[2];
}NET_EHOME_ETHERNET,*LPNET_EHOME_ETHERNET;
#define PASSWD_LEN                    16      //密码长度
typedef struct tagNET_EHOME_PPPOECFG
{
    DWORD        dwPPPoE;    //是否启用PPPOE：1代表使用；0代表不使用
    char            sPPPoEUser[NAME_LEN];    //PPPoE用户名
    char            sPPPoEPassword[PASSWD_LEN];    //PPPoE密码
    NET_EHOME_IPADDR    struPPPoEIP;        //PPPoE IP地址
}NET_EHOME_PPPOECFG,*LPNET_EHOME_PPPOECFG;

typedef struct tagNET_EHOME_NETWORK_CFG
{
    DWORD        dwSize;                //结构体大小
    NET_EHOME_ETHERNET        struEtherNet;    //以太网口
    NET_EHOME_IPADDR        struGateWayIP;//网关地址
    NET_EHOME_IPADDR        struMultiCastIP;//多播地址
    NET_EHOME_IPADDR        struDDNSServer1IP;//DDNS1服务器IP
    NET_EHOME_IPADDR        struDDNSServer2IP;//DDNS2服务器IP
    NET_EHOME_IPADDR        struAlarmHostIP;    //报警主机IP
    WORD        wAlarmHostPort;    //报警主机端口号
    WORD        wIPResolverPort;    //解析服务器端口
    NET_EHOME_IPADDR        struIPResolver; //解析服务器地址
    NET_EHOME_PPPOECFG        struPPPoE;    //PPPoE参数
    WORD        wHTTPPort;;        //Http端口
    BYTE        byRes[674];        //保留
}NET_EHOME_NETWORK_CFG,*LPNET_EHOME_NETWORK_CFG;

typedef struct tagNET_EHOME_COMPRESSION_COND
{
    DWORD        dwSize;    //结构体大小
    DWORD        dwChannelNum;    //通道号，从1开始
    BYTE        byCompressionType;//码流类型，1-主码流，2-子码流，3-三码流
    BYTE        byRes[23];
}NET_EHOME_COMPRESSION_COND,*LPNET_EHOME_COMPRESSION_COND;

typedef struct tagNET_EHOME_COMPRESSION_CFG
{
    DWORD        dwSize;    //结构体大小
    BYTE        byStreamType;    //码流类型
    BYTE        byPicQuality;    //图像质量0:最高 1:较高 2:中等 3:低 4:较低 5:最低
    BYTE        byBitRateType;    //码率类型0:变码率 1:定码率
    BYTE        byRes1;    //保留
    DWORD        dwResolution;    //分辨率，0:DCIF 1:CIF 2:QCIF 3:4CIF 4:2CIF 6:QVGA（320x240） 16:VGA 17:UXGA 18:SVGA 19:HD720p 20:hd900 21:XVGA    22:SXGAp(1360*1024)
    //27:1080P(1920*1080)    28:2560x1920 /*500W*/    29:1600x304    30:2048x1536 /*300W*/
    //31:2448x2048/*500W*/        32:2448x1200        33:2448x800    34:XGA/*(1024*768)*/
    //35:SXGA/*(1280*1024)*/    36:WD1/*(960*576/960*480)*/    37:HD1080I    38-WXGA(1440*900)，
    //39-HD_F(1920*1080/1280*720)，40-HD_H(1920*540/1280*360)，  41-HD_Q(960*540/630*360)，  
    //42-2336*1744，    43-1920*1456，44-2592*2048，    45-3296*2472，46-1376*768，47-1366*768,                 
    //48-1360*768,  49-WSXGA+，50-720*720，51-1280*1280，52-2048*768，53-2048*2048
    //54-2560x2048,  55-3072x2048 ,  56-2304*1296  57-WXGA(1280*800),  58-1600*600    
    //59-2592*1944  60-2752*2208,    61-384*288,    62-4000*3000,  63-4096*2160,  64-3840*2160,
    //65-4000*2250, 66-3072*1728,
    DWORD        dwVideoBitRate;    //0-32K 1-48k 2-64K 3-80K 4-96K 5-128K 6-160k 7-192K 8-224K 9-256K 10-320K 11-384K 12-448K 13-512K 14-640K 15-768K 16-896K 17-1024K 18-1280K 19-1536K 20-1792K 21-2048K 22-自定义
    DWORD        dwMaxBitRate;    //自定义码率
    DWORD        dwVideoFrameRate;    //视频帧率（4字节整数）0:全帧率 1:1/16 2:1/8 3:1/4 4:1/2 5:1 6:2 7:4 8:6 9:8 10:10 11:12 12:16 13:20 14:15 15:18 16:22
    WORD        wIntervalFrameI;    // I帧间隔，范围1~400
    BYTE        byIntervalBPFrame; //帧类型 0:BBP 1：BP 2:P
    BYTE        byRes[41];    //保留
}NET_EHOME_COMPRESSION_CFG,*LPNET_EHOME_COMPRESSION_CFG;

#define    MAX_TIME_SEGMENT        8   //时间段,ehome协议只支持4个
#define MAX_ANALOG_ALARMOUT     32      //最大32路模拟报警输出
#define MAX_ANALOG_CHANNUM      32      //最大32个模拟通道
#define MAX_DIGIT_CHANNUM    480/*512-32*/      //最大480个数字通道,与网络库保持

typedef struct tagNET_EHOME_ALARM_TIME_COND
{
    DWORD        dwSize;    //结构体大小
    BYTE        byAlarmType;    //报警类型，0-移动侦测，1-视频丢失，2-遮挡报警，3-报警输入，4-报警输出，9-客流量
    BYTE        byWeekday;    //0-Mon，1-Tues，2-Wed，3-Thur，4-Fri，5-Sat，6-Sun
    BYTE        byRes1[2];
    DWORD        dwChannel;    //通道号，从1开始
    BYTE        byRes2[20];
}NET_EHOME_ALARM_TIME_COND,*LPNET_EHOME_ALARM_TIME_COND;

typedef struct tagNET_EHOME_SCHEDTIME
{
    BYTE        byStartHour;    //开始时间：时
    BYTE        byStartMin;    //开始时间：分
    BYTE        byStopHour;    //结束时间：时
    BYTE        byStopMin;    //结束时间：分
}NET_EHOME_SCHEDTIME,*LPNET_EHOME_SCHEDTIME;

typedef struct tagNET_EHOME_ALARM_TIME_CFG
{
    DWORD        dwSize;    //结构体大小
    NET_EHOME_SCHEDTIME    struSchedTime[MAX_TIME_SEGMENT];//布防时间段
    BYTE        bySchedTimeCount;    //只读，布防时间段数
    BYTE        byRes[43];
}NET_EHOME_ALARM_TIME_CFG,*LPNET_EHOME_ALARM_TIME_CFG;

typedef struct tagNET_EHOME_ALARMOUT_CFG
{
    DWORD        dwSize;    //结构体大小
    BYTE        sAlarmOutName[NAME_LEN];    //报警输出名称
    WORD        wAlarmOutDelay;    //输出延时：0：5秒；1：10秒； 2：30秒； 3：1分； 4：2分； 5：5分； 6：10分； 7：最大；
    BYTE        byRes[26];    //保留
}NET_EHOME_ALARMOUT_CFG,*LPNET_EHOME_ALARMOUT_CFG;

typedef struct tagNET_EHOME_ALARMOUT_STATUS_CFG
{
    DWORD        dwSize;    //结构体大小
    BYTE        byAlarmOutStatus;    //1-开启报警输出，0-关闭报警输出
    BYTE        byRes[11];
}NET_EHOME_ALARMOUT_STATUS_CFG,*LPNET_EHOME_ALARMOUT_STATUS_CFG;

typedef struct tagNET_EHOME_ALARMIN_COND
{
    DWORD        dwSize;    //结构体大小
    DWORD        dwAlarmInNum;    //报警编号，从1开始
    DWORD        dwPTZChan;// PTZ联动视频通道编号,从1开始
    BYTE        byRes[20];
}NET_EHOME_ALARMIN_COND,*LPNET_EHOME_ALARMIN_COND;

typedef struct tagNET_EHOME_LINKAGE_ALARMOUT
{
    DWORD        dwAnalogAlarmOutNum;    //只读，模拟报警数量
    BYTE        byAnalogAlarmOut[MAX_ANALOG_ALARMOUT];    //模拟报警输出，0：不使用；1：使用
    BYTE        byRes[5000];    //保留，协议里面没有IP报警输出联动，接口上位置留出来
}NET_EHOME_LINKAGE_ALARMOUT,*LPNET_EHOME_LINKAGE_ALARMOUT;

typedef struct tagNET_EHOME_LINKAGE_PTZ
{
    BYTE    byUsePreset;    //是否调用预置点，0：不使用；1：使用
    BYTE    byUseCurise;    //是否调用巡航，0：不使用；1：使用
    BYTE    byUseTrack;    //是否调用轨迹，0：不使用；1：使用
    BYTE    byRes1;        //保留
    WORD    wPresetNo;    //预置点号，范围：1~256，协议中规定是1～256，实际已有设备支持300
    WORD    wCuriseNo;    //巡航路径号，范围：1~16
    WORD    wTrackNo;    //轨迹号，范围：1~16
    BYTE    byRes2[6];    //保留
}NET_EHOME_LINKAGE_PTZ,*LPNET_EHOME_LINKAGE_PTZ;

typedef struct tagNET_EHOME_ALARMIN_LINKAGE_TYPE
{
    BYTE    byMonitorAlarm;    //监视器上警告，0：不使用；1：使用
    BYTE    bySoundAlarm;    //声音报警，0：不使用；1：使用
    BYTE    byUpload;        //上传中心，0：不使用；1：使用
    BYTE    byAlarmout;        //触发报警输出，0：不使用；1：使用-
    BYTE    byEmail;            //邮件联动，0：不使用；1：使用
    BYTE    byRes1[3];        //保留
    NET_EHOME_LINKAGE_PTZ    struPTZLinkage;    //PTZ联动
    NET_EHOME_LINKAGE_ALARMOUT    struAlarmOut;    //报警输出联动
    BYTE    byRes[128];
}NET_EHOME_ALARMIN_LINKAGE_TYPE,*LPNET_EHOME_ALARMIN_LINKAGE_TYPE;

typedef    struct    tagNET_EHOME_RECORD_CHAN
{
    BYTE    byAnalogChanNum;    //只读，模拟通道数
    BYTE    byAnalogChan[MAX_ANALOG_CHANNUM];    //模拟通道，0：不使用；1：使用
    BYTE    byRes1[3];    //保留
    WORD    wDigitChanNum;    //只读，数字通道数
    BYTE    byDigitChan[MAX_DIGIT_CHANNUM];    //数字通道，0：不使用；1：使用
    BYTE    byRes2[62];    //保留
}NET_EHOME_RECORD_CHAN,*LPNET_EHOME_RECORD_CHAN;

typedef struct tagNET_EHOME_ALARMIN_CFG
{
    DWORD        dwSize;    //结构体大小
    BYTE        sAlarmInName[NAME_LEN];    //报警输入名称
    BYTE        byAlarmInType;    //报警器类型：0：常开；1：常闭
    BYTE        byUseAlarmIn;    //是否处理，0：不使用；1：使用
    BYTE        byRes1[2];            //保留
    NET_EHOME_ALARMIN_LINKAGE_TYPE    struLinkageType;    //联动模式
    NET_EHOME_RECORD_CHAN    struRecordChan;    //关联录像通道
    BYTE        byRes2[128];        //保留
}NET_EHOME_ALARMIN_CFG,*LPNET_EHOME_ALARMIN_CFG;

typedef    struct    tagNET_EHOME_MANUAL_IOOUT_CTRL
{
    DWORD        dwSize;    //结构体大小
    DWORD        dwChan;        //IO输出编号，从1开始
    DWORD        dwDelayTime;        //报警输出持续时间（秒），值为0表示一直输出，否则按指定时间长度进行输出。
    BYTE        byAction;        //0-关闭报警输出,1-开启报警输出
    BYTE        byRes[19];
}NET_EHOME_MANUAL_IOOUT_CTRL,*LPNET_EHOME_MANUAL_IOOUT_CTRL;

typedef struct tagNET_EHOME_IMAGE_CFG
{
    DWORD       dwSize;    //结构体大小
    BYTE        byHue;    //色调，0～255
    BYTE        byContrast;    //对比度,0~255
    BYTE        byBright;        //亮度，0～255
    BYTE        bySaturation;    //饱和度
    BYTE        byRes[24];
}NET_EHOME_IMAGE_CFG,*LPNET_EHOME_IMAGE_CFG;


typedef struct tagBINARY_DATA_INFO
{
    DWORD dwMagic;
    DWORD dwCommand;  //命令码
    DWORD dwSequence; //序号
    DWORD dwStatus;   //状态值
    DWORD dwBufLen;
    BYTE  byMsgType;    /*报文类型，0-无效，1-请求报文，2-应答报文*/
    char  sSerialNumber[12];//设备序列号，尚未使用
    //char  sDeviceID[16];//设备ID
    BYTE  byVersion;
    BYTE  byRes[2];
} BINARY_DATA_INFO, *LPBINARY_DATA_INFO;


#define    NET_EHOME_GET_NETWORK_CFG        5    //获取网络参数
#define    NET_EHOME_SET_NETWORK_CFG        6    //设置网络参数
#define    NET_EHOME_GET_COMPRESSION_CFG    7    //获取压缩参数
#define    NET_EHOME_SET_COMPRESSION_CFG    8    //设置压缩参数
#define    NET_EHOME_GET_IMAGE_CFG            9    //获取图像参数
#define    NET_EHOME_SET_IMAGE_CFG            10    //设置图像参数
#define    NET_EHOME_GET_ALARMIN_CFG        11        //获取报警输入参数
#define    NET_EHOME_SET_ALARMIN_CFG        12    //设置报警输入参数
#define    NET_EHOME_GET_ALARM_TIME_CFG    13    //获取报警布防时间参数
#define    NET_EHOME_SET_ALARM_TIME_CFG    14    //设置报警布防时间参数
#define    NET_EHOME_GET_ALARMOUT_CFG        15    //获取报警输出参数
#define    NET_EHOME_SET_ALARMOUT_CFG        16    //设置报警输出参数
#define    NET_EHOME_GET_ALARMOUT_STATUS_CFG        17    //获取报警输出状态参数
#define    NET_EHOME_SET_ALARMOUT_STATUS_CFG        18    //设置报警输出状态参数
#define    NET_EHOME_MANUAL_IOOUT            19    //手动控制IO输出

typedef enum tagNET_CMS_ENUM_PROXY_TYPE
{
	ENUM_PROXY_TYPE_NETSDK = 0,	//NetSDK代理
	ENUM_PROXY_TYPE_HTTP					//HTTP代理
}NET_CMS_ENUM_PROXY_TYPE;

typedef struct tagNET_EHOME_PT_PARAM
{
	NET_EHOME_IPADDRESS struIP;   //监听的IP和端口
	BYTE	byProtocolType;							//协议类型，0-TCP
	BYTE	byProxyType;									//代理类型，0-NetSDK代理，1-HTTP代理
	BYTE	byRes[2];
}NET_EHOME_PT_PARAM, *LPNET_EHOME_Proxy_PARAM;

typedef struct tagNET_EHOME_PASSTHROUGH_PARAM
{
	DWORD	dwSequence;	//报文序号
	DWORD	dwUUID;		//会话ID
	BYTE			byRes[64];
}NET_EHOME_PASSTHROUGH_PARAM, *LPNET_EHOME_PASSTHROUGH_PARAM;

typedef struct tagNET_EHOME_PTXML_PARAM
{
    void*   pRequestUrl;        //请求URL
    DWORD   dwRequestUrlLen;    //请求URL长度
    void*   pCondBuffer;        //条件缓冲区（XML格式数据）
    DWORD   dwCondSize;         //条件缓冲区大小
    void*   pInBuffer;          //输入缓冲区（XML格式数据）
    DWORD   dwInSize;           //输入缓冲区大小
    void*   pOutBuffer;         //输出缓冲区（XML格式数据）
    DWORD   dwOutSize;          //输出缓冲区大小
    DWORD   dwReturnedXMLLen;   //实际从设备接收到的XML数据的长度
    BYTE    byRes[32];          //保留
}NET_EHOME_PTXML_PARAM, *LPNET_EHOME_PTXML_PARAM;

NET_DVR_API LONG CALLBACK NET_ECMS_StartListenProxy(LPNET_EHOME_Proxy_PARAM lpStru);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopListenProxy(LONG lListenHandle, DWORD dwProxyType = 0);
NET_DVR_API LONG CALLBACK NET_ECMS_ConvertProtocolHttpToPassthrough(void* pSrcBuffer, DWORD dwSrcBufLen, void* pDestBuffer, DWORD dwDestBufLen, LPNET_EHOME_PASSTHROUGH_PARAM lpParam, BOOL bToPassthrough = TRUE);
typedef void (CALLBACK* PASSTHROUGHDATACALLBACK)(DWORD dwProxyType, LONG lListenHandle, void* pDeviceID, DWORD dwDevIDLen, void* pDataBuffer, DWORD dwDataLen, void* pUser);
NET_DVR_API BOOL CALLBACK NET_ECMS_SetPassthroughDataCallback(PASSTHROUGHDATACALLBACK fnPassthroughDataCb, void* pUser, DWORD dwProxyType = 0);
NET_DVR_API BOOL CALLBACK NET_ECMS_SendPassthroughData(void* pDataBuffer, DWORD dDataLen, DWORD dwProxyType = 0);
NET_DVR_API BOOL CALLBACK NET_ECMS_GetPTXMLConfig(LONG iUserID, LPNET_EHOME_PTXML_PARAM lpPTXMLParam);
NET_DVR_API BOOL CALLBACK NET_ECMS_PutPTXMLConfig(LONG iUserID, LPNET_EHOME_PTXML_PARAM lpPTXMLParam);
NET_DVR_API BOOL CALLBACK NET_ECMS_PostPTXMLConfig(LONG iUserID, LPNET_EHOME_PTXML_PARAM lpPTXMLParam);
NET_DVR_API BOOL CALLBACK NET_ECMS_DeletePTXMLConfig(LONG lUserID, LPNET_EHOME_PTXML_PARAM lpPTXMLParam);


NET_DVR_API BOOL CALLBACK NET_ECMS_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType, void* const lpInBuff);
NET_DVR_API BOOL CALLBACK NET_ECMS_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType,void *lpOutBuff);

typedef struct tagNET_EHOME_XML_REMOTE_CTRL_PARAM
{
    DWORD dwSize;
    void* lpInbuffer;          //控制参数缓冲区
    DWORD  dwInBufferSize;      //控制参数缓冲区长度
    DWORD  dwSendTimeOut;  //数据发送超时时间,单位ms，默认5s
    DWORD  dwRecvTimeOut;  //数据接收超时时间,单位ms，默认5s
    void *lpOutBuffer;     //输出缓冲区
    DWORD dwOutBufferSize;  //输出缓冲区大小
    void *lpStatusBuffer;   //状态缓冲区,若不需要可置为NULL
    DWORD dwStatusBufferSize;  //状态缓冲区大小
    BYTE   byRes[16];
}NET_EHOME_XML_REMOTE_CTRL_PARAM, *LPNET_EHOME_XML_REMOTE_CTRL_PARAM;
NET_DVR_API BOOL CALLBACK NET_ECMS_XMLRemoteControl(LONG lUserID, LPNET_EHOME_XML_REMOTE_CTRL_PARAM lpCtrlParam, DWORD dwCtrlSize);


#define    EHOME_CMSALARM_EXCEPTION      0x105  //CMS接收报警异常
NET_DVR_API BOOL CALLBACK NET_ECMS_SetExceptionCallBack(DWORD dwMessage, HANDLE hWnd, void (CALLBACK* fExceptionCallBack)(DWORD dwType, LONG iUserID, LONG iHandle, void* pUser), void* pUser);

NET_DVR_API BOOL CALLBACK NET_ECMS_TranBuf(LONG lUserID, DWORD dwLength, void *pBuf);


typedef enum
{
    LONG_CFG_CREATED,  //长连接建立成功
    LONG_CFG_CREATE_FAIL,   //长连接建立失败
    LONG_CFG_DATA,  //长连接普通数据
    LONG_CFG_TERMINATE  //长连接销毁消息
}LONG_LINK_MSG;

typedef BOOL(CALLBACK *LongConfigCallBack)(LONG iHandle, LONG_LINK_MSG enMsg, void *pOutBuffer, DWORD dwOutLen, void *pUser);

typedef struct tagNET_EHOME_LONG_CFG_INPUT
{
    LongConfigCallBack fnDataCallBack;
    void *pUser;
    BYTE byRes[32];
}NET_EHOME_LONG_CFG_INPUT, *LPNET_EHOME_LONG_CFG_INPUT;

typedef struct tagNET_EHOME_LONG_CFG_SEND
{
    void *pDataBuffer;
    DWORD dwDataLen;
    BYTE byRes[32];
}NET_EHOME_LONG_CFG_SEND, *LPNET_EHOME_LONG_CFG_SEND;

NET_DVR_API LONG CALLBACK NET_ECMS_LongConfigCreate(LONG lUserlD, LPNET_EHOME_LONG_CFG_INPUT pLongCfgInput);

NET_DVR_API BOOL CALLBACK NET_ECMS_LongConfigSend(LONG lHandle, LPNET_EHOME_LONG_CFG_SEND pSend);

NET_DVR_API BOOL CALLBACK NET_ECMS_LongConfigDestory(LONG lHandle);

#endif //_HC_EHOME_CMS_H_
