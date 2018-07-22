#ifndef _HC_EHOME_CMS_H_
#define _HC_EHOME_CMS_H_

#include "HCEHomePublic.h"

//��ʼ��������ʼ��
NET_DVR_API BOOL  CALLBACK NET_ECMS_Init();
NET_DVR_API BOOL  CALLBACK NET_ECMS_Fini();

//��ȡ������
NET_DVR_API DWORD CALLBACK NET_ECMS_GetLastError();

//��ȡ�汾��
NET_DVR_API DWORD CALLBACK NET_ECMS_GetBuildVersion();

//�����رռ���
typedef enum tagNET_EHOME_REGISTER_TYPE{
    ENUM_UNKNOWN = -1,   
    ENUM_DEV_ON = 0,             //�豸���߻ص�
    ENUM_DEV_OFF,                //�豸���߻ص�
    ENUM_DEV_ADDRESS_CHANGED     //�豸��ַ�����仯
}NET_EHOME_REGISTER_TYPE;

typedef BOOL (CALLBACK * DEVICE_REGISTER_CB)(LONG lUserID, DWORD dwDataType, void *pOutBuffer, DWORD dwOutLen, 
                                                 void *pInBuffer, DWORD dwInLen, void *pUser);

typedef struct tagNET_EHOME_CMS_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struAddress;  //���ؼ�����Ϣ��IPΪ0.0.0.0������£�Ĭ��Ϊ���ص�ַ���������������£�Ĭ��Ϊ�Ӳ���ϵͳ��ȡ���ĵ�һ��
    DEVICE_REGISTER_CB  fnCB; //������Ϣ�ص�����
    void * pUserData;   //�û�����
    BYTE  byRes[32]; 
}NET_EHOME_CMS_LISTEN_PARAM, *LPNET_EHOME_CMS_LISTEN_PARAM;

//�����رռ���
NET_DVR_API LONG CALLBACK NET_ECMS_StartListen(LPNET_EHOME_CMS_LISTEN_PARAM lpCMSListenPara);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopListen(LONG iHandle);
//ע���豸
NET_DVR_API BOOL CALLBACK NET_ECMS_ForceLogout(LONG lUserID);
NET_DVR_API BOOL CALLBACK NET_ECMS_SetLogToFile(DWORD iLogLevel, char *strLogDir, BOOL bAutoDel);

#define FIRMWARE_VER_LEN 24
#define CODE_LEN   8
typedef struct tagNET_EHOME_DEV_REG_INFO
{
    DWORD  dwSize; 
    DWORD  dwNetUnitType;            //����EHomeSDKЭ��Ԥ����Ŀǰû������
    BYTE   byDeviceID[MAX_DEVICE_ID_LEN]; //�豸ID
    BYTE   byFirmwareVersion[24];    //�̼��汾
    NET_EHOME_IPADDRESS struDevAdd;         //�豸ע�������ǣ��豸�ı��ص�ַ
    DWORD  dwDevType;                  //�豸����
    DWORD  dwManufacture;              //�豸���Ҵ���
    BYTE   byPassWord[32];             //�豸��½CMS�����룬���û����и������������֤
    BYTE   sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];    //�豸���кţ��������к�
    BYTE   byRes[52];
}NET_EHOME_DEV_REG_INFO, *LPNET_EHOME_DEV_REG_INFO;

typedef struct tagNET_EHOME_DEV_REG_INFO_V12
{
    NET_EHOME_DEV_REG_INFO struRegInfo;
    NET_EHOME_IPADDRESS struRegAddr;         //�豸ע��ķ�������ַ
    BYTE   byRes[256];
}NET_EHOME_DEV_REG_INFO_V12, *LPNET_EHOME_DEV_REG_INFO_V12;

typedef struct tagNET_EHOME_BLACKLIST_SEVER
{
    NET_EHOME_IPADDRESS struAdd;          //��������ַ
    BYTE    byServerName[NAME_LEN/*32*/];       //����������
    BYTE    byUserName[NAME_LEN/*32*/];             //�û���
    BYTE    byPassWord[NAME_LEN/*32*/];             //����
    BYTE   byRes[64];
}NET_EHOME_BLACKLIST_SEVER, *LPNET_EHOME_BLACKLIST_SEVER;

typedef struct tagNET_EHOME_SERVER_INFO
{
    DWORD                  dwSize;
    DWORD                  dwKeepAliveSec;            //�����������λ����,0:Ĭ��Ϊ15S��
    DWORD                  dwTimeOutCount;         //������ʱ������0��Ĭ��Ϊ6��
    NET_EHOME_IPADDRESS         struTCPAlarmSever;      //������������ַ��TCPЭ�飩
    NET_EHOME_IPADDRESS         struUDPAlarmSever;        //������������ַ��UDPЭ�飩
    DWORD                  dwAlarmServerType;        //��������������0-ֻ֧��UDPЭ���ϱ���1-֧��UDP��TCP����Э���ϱ�
    NET_EHOME_IPADDRESS         struNTPSever;            //NTP��������ַ
    DWORD                  dwNTPInterval;            //NTPУʱ�������λ���룩
    NET_EHOME_IPADDRESS         struPictureSever;       //ͼƬ��������ַ
    DWORD                  dwPicServerType;        //ͼƬ����������ͼƬ���������ͣ�1-VRBͼƬ��������0-TomcatͼƬ����,2-�ƴ洢3,3-KMS
    NET_EHOME_BLACKLIST_SEVER   struBlackListServer;    //������������
    BYTE                   byRes[128];
}NET_EHOME_SERVER_INFO, *LPNET_EHOME_SERVER_INFO;


//-----------------------------------------------------------------------------------------------------------
//�����Խ�

typedef void(CALLBACK *fVoiceDataCallBack)(LONG iVoiceHandle, char *pRecvDataBuffer, DWORD dwBufSize, DWORD dwEncodeType, BYTE byAudioFlag, void* pUser);

typedef struct tagNET_EHOME_VOICETALK_PARA
{
    BOOL           bNeedCBNoEncData; //��Ҫ�ص����������ͣ�0-�����������1-����ǰ����������ת��ʱ��֧�֣�
    fVoiceDataCallBack  cbVoiceDataCallBack; //���ڻص���Ƶ���ݵĻص�����
    DWORD          dwEncodeType;    //SDK��ֵ,SDK��������������,0- OggVorbis��1-G711U��2-G711A��3-G726��4-AAC��5-MP2L2��6-PCM
    void*          pUser;    //�û�����
    BYTE           byVoiceTalk;    //0-�����Խ�,1-����ת��
    BYTE           byDevAudioEnc;  //����������豸����Ƶ���뷽ʽ 0- OggVorbis��1-G711U��2-G711A��3-G726��4-AAC��5-MP2L2��6-PCM
    BYTE           byRes[62];//Reserved, set as 0. 0
} NET_EHOME_VOICETALK_PARA,*LPNET_EHOME_VOICETALK_PARA;

typedef struct tagNET_EHOME_VOICE_TALK_IN
{
    DWORD dwVoiceChan;                   //ͨ����
    NET_EHOME_IPADDRESS struStreamSever; //��ý���ַ
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
    void*        pCondBuf;    //[in]����������ָ�룬���ʾͨ���ŵ�
    DWORD        dwCondSize; //[in]��pCondBufָ������ݴ�С
    void*        pInBuf;        //[in]������ʱ��Ҫ�õ���ָ��ṹ���ָ��
    DWORD        dwInSize;    //[in], pInBufָ������ݴ�С
    void*        pOutBuf;        //[out]����ȡʱ��Ҫ�õ���ָ��ṹ���ָ�룬�ڴ����ϲ����
    DWORD        dwOutSize;    //[in]����ȡʱ��Ҫ�õ�����ʾpOutBufָ����ڴ��С�� 
    BYTE         byRes[40];    //����
}NET_EHOME_CONFIG, *LPNET_EHOME_CONFIG;

typedef struct tagNET_EHOME_XML_CFG 
{
    void*      pCmdBuf;    //�ַ�����ʽ����
    DWORD      dwCmdLen;   //pCmdBuf����
    void*      pInBuf;     //��������
    DWORD      dwInSize;   //�������ݳ���
    void*      pOutBuf;    //�������
    DWORD      dwOutSize;  //�������������
    DWORD      dwSendTimeOut;  //���ݷ��ͳ�ʱʱ��,��λms��Ĭ��5s
    DWORD      dwRecvTimeOut;  //���ݽ��ճ�ʱʱ��,��λms��Ĭ��5s
    void*      pStatusBuf;     //���ص�״̬����(XML��ʽ),�������Ҫ,������NULL
    DWORD      dwStatusSize;   //״̬��������С(�ڴ��С)
    BYTE       byRes[24];
}NET_EHOME_XML_CFG, *LPNET_EHOME_XML_CFG;

#define    MAX_SERIALNO_LEN        128    //���к���󳤶�
#define    MAX_PHOMENUM_LEN        32    //�ֻ�������󳤶�
#define    MAX_DEVICE_NAME_LEN     32//�豸���Ƴ���

typedef struct tagNET_DVR_DVR_TYPE_NAME
{
    DWORD dwDVRType;
    char byDevName[24];  
}NET_DVR_TYPE_NAME;

static const NET_DVR_TYPE_NAME DVRTypeName[] = 
{
    {0,"UNKNOWN TYPE"},      /*����δ�����dvr���ͷ���NETRET_DVR*/
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
    {13,"DVS_A"},              /* ��ATAӲ�̵�DVS */
    {14,"DVR_HC_S"},              /* 8000HC-S */
    {15,"DVR_HT_S"},              /* 8000HT-S */
    {16,"DVR_HF_S"},              /* 8000HF-S */
    {17,"DVR_HS_S"},              /* 8000HS-S */
    {18,"ATMDVR_S"},              /* ATM-S */
    {19,"DVR_7000H"},                /*7000Hϵ��*/
    {20,"DEC_MAT"},              /*��·������*/
    {21,"DVR_MOBILE"},                /* mobile DVR */                 
    {22,"DVR_HD_S"},              /* 8000HD-S */
    {23,"DVR_HD_SL"},                /* 8000HD-SL */
    {24,"DVR_HC_SL"},                /* 8000HC-SL */
    {25,"DVR_HS_ST"},                /* 8000HS_ST */
    {26,"DVS_HW"},              /* 6000HW */
    {27,"DS630X_D"},             /* ��·������ */
    {28,"DS640X_HD"},                /*640X���������*/
    {29,"DS610X_D"},              /*610X������*/
    {30,"IPCAM"},                /*IP �����*/
    {31,"MEGA_IPCAM"},                /*����IP����� 852F&852MF*/
    {32,"IPCAM_862MF"},                /*862MF���Խ���9000�豸*/
    {35,"ITCCAM"},                /*���ܸ������������*/
    {36,"IVS_IPCAM"},          /*���ܷ����������������*/
    {38,"ZOOMCAM"},            /*һ���*/
    {40,"IPDOME"},              /*IP �������*/
    {41,"IPDOME_MEGA200"},              /*IP 200��������*/
    {42,"IPDOME_MEGA130"},              /*IP 130��������*/
    {43,"IPDOME_AI"},              /*IP �������ܿ���*/ 
    {44,"TII_IPCAM"},          /*�����ȳ��������*/
    {50,"IPMOD"},                /*IP ģ��*/
    {59,"DS64XXHD_T"},                //64-T���������
    {60,"IDS6501_HF_P"},    // 6501 ����
    {61,"IDS6101_HF_A"},              //����ATM
    {62,"IDS6002_HF_B"},          //˫�����٣�DS6002-HF/B
    {63,"IDS6101_HF_B"},          //��Ϊ������DS6101-HF/B
    {64,"IDS52XX"},          //���ܷ�����IVMS
    {65,"IDS90XX"},                // 9000����
    {67,"IDS8104_AHL_S_H"},              // ��������ʶ�� ATM
    {68,"IDS91XX"},                // 9100����
    {69,"IIP_CAM_B"},  //������ΪIP�����
    {70,"IIP_CAM_F"},  //��������IP�����
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
    {93,"IDS90XX_A"},                // 9000���� ATM
    {94,"IDS91XX_A"},                // 9100���� ATM
    {95,"DS95XX_N_S"},              /*DS95XX_N_S NVR �����κ����*/
    {96,"DS96XX_N_SH"},              /*DS96XX_N_SH NVR*/
    {97,"DS90XX_HF_SH"},              /*DS90XX_HF_SH */   
    {98,"DS91XX_HF_SH"},              /*DS91XX_HF_SH */
    {100,"DS_B10_XY"},             /*��Ƶ�ۺ�ƽ̨�豸�ͺ�(X:�����Ƭ����Y:�����Ƭ��)*/
    {101,"DS_6504HF_B10"},             /*��Ƶ�ۺ�ƽ̨�ڲ�������*/
    {102,"DS_6504D_B10"},             /*��Ƶ�ۺ�ƽ̨�ڲ�������*/
    {103,"DS_1832_B10"},             /*��Ƶ�ۺ�ƽ̨�ڲ������*/
    {104,"DS_6401HFH_B10"},             /*��Ƶ�ۺ�ƽ̨�ڲ����˰�*/
    {105,"DS_65XXHC"},                //65XXHC DVS
    {106,"DS_65XXHC_S"},                //65XXHC-SATA DVS
    {107,"DS_65XXHF"},                //65XXHF DVS
    {108,"DS_65XXHF_S"},                //65XXHF-SATA DVS
    {109,"DS_6500HF_B"},             //65 rack DVS
    {110,"IVMS_6200_C"},            // iVMS-6200(/C)  
    {111,"IVMS_6200_B"},             // iVMS-6200(/B)
    {112,"DS_72XXHV_ST"},                //72XXHV_ST15 DVR
    {113,"DS_72XXHV_ST"},                //72XXHV_ST20 DVR,�������豸��ʾһ������
    {114,"IVMS_6200_T"},             // IVMS-6200(/T)
    {115,"IVMS_6200_BP"},             // IVMS-6200(/BP)
    {116,"DS_81XXHC_ST"},                //DS_81XXHC_ST
    {117,"DS_81XXHS_ST"},                //DS_81XXHS_ST
    {118,"DS_81XXAH_ST"},                //DS_81XXAH_ST
    {119,"DS_81XXAHF_ST"},                //DS_81XXAHF_ST
    {120,"DS_66XXDVS"},                //66XX DVS
    {121,"DS_1964_B10"},             /*��Ƶ�ۺ�ƽ̨�ڲ�������*/
    {122,"DS_B10N04_IN"},             /*��Ƶ�ۺ�ƽ̨�ڲ���������*/
    {123,"DS_B10N04_OUT"},             /*��Ƶ�ۺ�ƽ̨�ڲ��������*/
    {124,"DS_B10N04_INTEL"},             /*��Ƶ�ۺ�ƽ̨�ڲ�����*/
    {125,"DS_6408HFH_B10E_RM"},             //V6����
    {126,"DS_B10N64F1_RTM"},             //V6��������DSP
    {127,"DS_B10N64F1D_RTM"},             //V6������DSP
    {128,"DS_B10_SDS"},             //��Ƶ�ۺ�ƽ̨���������
    {129,"DS_B10_DS"},             //��Ƶ�ۺ�ƽ̨�������
    {130,"DS_6401HFH_B10V"},             //VGA���������
    {131,"DS_6504D_B10B"},             /*��Ƶ�ۺ�ƽ̨�ڲ����������*/
    {132,"DS_6504D_B10H"},             /*��Ƶ�ۺ�ƽ̨�ڲ����������*/
    {133,"DS_6504D_B10V"},             /*��Ƶ�ۺ�ƽ̨�ڲ�VGA������*/
    {134,"DS_6408HFH_B10S"},             //��Ƶ�ۺ�ƽ̨SDI�Ӱ� 
    {135,"DS_18XX_N"},             /* �����������*/ 
    //ע��121��140Ԥ������Ƶ�ۺ�ƽ̨ϵ�в�Ʒʹ��,141~160��������豸�ͱ����豸����
    {141,"DS_18XX_PTZ"},                /*����������Ʒ*/
    {142,"DS_19AXX"},                /*ͨ�ñ����������Ʒ*/
    {143,"DS_19BXX"},                /*���ñ�������*/
    {144,"DS_19CXX"},                /*�������б�������*/
    {145,"DS_19DXX"},                /*������ر�������*/    
    {146,"DS_19XX"},             /*1900ϵ�б�������*/ 
    {147,"DS_19SXX"},                /*��Ƶ��������*/
    {148, "DS_1HXX"},            /*CS���Ʒ*/ //������
    /**********************�豸���� end***********************/
    //2011-11-03 161~170����������豸ʹ��
    {161,"DS_C10H"},           /*����������*/
    {162,"DS_C10N_BI"},            //BNC������
    {163,"DS_C10N_DI"},            //rbg������
    {164,"DS_C10N_SI"},            //����������
    {165,"DS_C10N_DO"},            //��ʾ������
    {166,"DS_C10N_SERVER"},        //�ֲ�ʽ������
    // 171- 180  ��������ʹ��
    {171,"IDS_8104_AHFL_S_H"},             // 8104ATM 
    {172,"IDS_65XX_HF_A"},             // 65 ATM
    {173,"IDS90XX_HF_RH"},             // 9000 ����RH
    {174,"IDS91XX_HF_RH"},             // 9100 ����RH�豸
    {175,"IDS_65XX_HF_B"},             // 65 ��Ϊ����
    {176,"IDS_65XX_HF_P"},             // 65 ����ʶ��
    {177,"IVMS_6200_F"},     // IVMS-6200(/F)
    {178,"IVMS_6200_A"},             //iVMS-6200(/A)
    {179,"IVMS_6200_F_S"},   //iVMS-6200(/F_S)
    {181,"DS90XX_HF_RH"},             // 9000 RH
    {182,"DS91XX_HF_RH"},             // 9100 RH�豸
    {183,"DS78XX_S"},             // 78ϵ���豸
    {185,"DS81XXHW_S"},                // 81 Resolution 960 
    {186,"DS81XXHW_ST"},             // DS81XXHW_ST
    {187,"DS91XXHW_ST"},             // DS91XXHW_ST
    {188,"DS91XX_ST"},             // DS91XX_ST
    {189,"DS81XX_ST"},             // DS81XX_ST
    {190,"DS81XXH_ST"},             // DS81XXHDI_ST,DS81XXHE_ST ky2012
    {191,"DS73XXH_ST"},             // DS73XXHI_ST ky2012
    {192,"DS81XX_SH"},             // ��Ѷ81SH,81SHF
    {193,"DS81XX_SN"},             // ��Ѷ81SNL
    {194,"DS96XXN_ST"},             //NVR:DS96xxN_ST
    {195,"DS86XXN_ST"},             //NVR:DS86xxN_ST
    {196,"DS80XXHF_ST"},             //DVR:DS80xxHF_ST
    {197,"DS90XXHF_ST"},             //DVR:DS90xxHF_ST
    {198,"DS76XXN_ST"},             //NVR:DS76xxN_ST
    {199,"DS_9664N_RX"},         //Ƕ��ʽNVR(64·IPͨ������ģ��ͨ��)�����һ��X��T/H    
    {200,"ENCODER_SERVER"},            // ���뿨������
    {201,"DECODER_SERVER"},         // ���뿨������
    {202,"PCNVR_SERVER"},         // PCNVR�洢������
    {203,"CVR_SERVER"},         // ��ŵCVR�������Լ���������ΪDVR_S-1
    {204,"DS_91XXHFH_ST"},         // 91ϵ��HD-SDI����DVR
    {205,"DS_66XXHFH"},         // 66���������    
    {210,"TRAFFIC_TS_SERVER"},         //�ն˷�����
    {211,"TRAFFIC_VAR"},         //��Ƶ������¼��
    {212,"IPCALL"},              //IP���ӶԽ��ֻ�

    //2012-03-21 ����ֵ255�Ժ���豸��Ҫ������������У��������������Ϊ��ʹ��չЭ���ܻ�ȡ����ֵ255�����豸������
    //{2001, "DS90XXHW_ST"}        // DS90XXHW_ST���DVR
};

typedef struct tagNET_EHOME_DEVICE_INFO
{
    DWORD        dwSize;                //�ṹ���С
    DWORD        dwChannelNumber;     //ģ����Ƶͨ������
    DWORD        dwChannelAmount;    //����Ƶͨ������ģ��+IP��
    DWORD        dwDevType;            //�豸���ͣ�1-DVR��3-DVS��30-IPC��40-IPDOME������ֵ�ο�����NetSdk�豸���ͺŶ���ֵ
    DWORD        dwDiskNumber;        //�豸��ǰӲ����
    BYTE        sSerialNumber[MAX_SERIALNO_LEN];        //�豸���к�
    DWORD        dwAlarmInPortNum;    //ģ��ͨ�������������
    DWORD        dwAlarmInAmount;    //�豸�ܱ����������
    DWORD        dwAlarmOutPortNum;    //ģ��ͨ�������������
    DWORD        dwAlarmOutAmount;    //�豸�ܱ����������
    DWORD        dwStartChannel;        //��Ƶ��ʼͨ����
    DWORD        dwAudioChanNum;    //�����Խ�ͨ������
    DWORD        dwMaxDigitChannelNum;    //�豸֧�ֵ��������ͨ��·��
    DWORD        dwAudioEncType;        //�����Խ���Ƶ��ʽ��0- OggVorbis��1-G711U��2-G711A��3-G726��4-AAC��5-MP2L2,6-PCM
    BYTE        sSIMCardSN[MAX_SERIALNO_LEN];    //�����豸��չ��SIM�����к�
    BYTE        sSIMCardPhoneNum[MAX_PHOMENUM_LEN];    //������չ��SIM���ֻ�����
    DWORD        dwSupportZeroChan;    // SupportZeroChan:֧����ͨ���ĸ�����0-��֧�֣�1-֧��1·��2-֧��2·���Դ�����
    DWORD        dwStartZeroChan;        //��ͨ����ʼ��ţ�Ĭ��10000
    DWORD        dwSmartType;            //�������ͣ�0-smart��1-רҵ���ܣ�Ĭ��0-smart
    BYTE        byRes[160];            //����
}NET_EHOME_DEVICE_INFO,*LPNET_EHOME_DEVICE_INFO;

#define    MAX_VERSION_LEN            32  //�汾��������
typedef struct tagNET_EHOME_VERSION_INFO
{
    DWORD        dwSize;                //�ṹ���С
    BYTE        sSoftwareVersion[MAX_VERSION_LEN];    //���ذ汾
    BYTE        sDSPSoftwareVersion[MAX_VERSION_LEN];//����汾
    BYTE        sPanelVersion[MAX_VERSION_LEN];    //���汾
    BYTE        sHardwareVersion[MAX_VERSION_LEN];    //Ӳ���汾
    BYTE        byRes[124];
}NET_EHOME_VERSION_INFO,*LPNET_EHOME_VERSION_INFO;

typedef struct tagNET_EHOME_DEVICE_CFG
{
    DWORD        dwSize;                //�ṹ���С
    BYTE        sServerName[MAX_DEVICE_NAME_LEN];    //�豸����
    DWORD        dwServerID;            //�豸�ţ�ң�������:0~255��
    DWORD        dwRecycleRecord;        //�Ƿ�ѭ��¼��0����ѭ����1����ѭ��
    DWORD        dwServerType;        //�豸���ͣ�ͬDevType�����ڹ�˾�豸�ͺ��м��ٸ�����Э����ֻʹ��ͨ���ͺţ�1-DVR��3-DVS��30-IPC��40-IPDOME
    DWORD        dwChannelNum;        //ͨ������������ģ��+IPͨ����ֻ����
    DWORD        dwHardDiskNum;        //Ӳ�̸�����ֻ����
    DWORD        dwAlarmInNum;        //�������������ģ��ͨ������ֻ����
    DWORD        dwAlarmOutNum;        //�������������ģ��ͨ������ֻ����
    DWORD        dwRS232Num;        //232���ڸ�����ֻ����
    DWORD        dwRS485Num;        //485���ڸ�����ֻ����
    DWORD        dwNetworkPortNum;    //����ڸ�����ֻ����
    DWORD        dwAuxoutNum;        //���ڸ�����ֻ����
    DWORD        dwAudioNum;        //�����ڸ�����ֻ����
    BYTE        sSerialNumber[MAX_SERIALNO_LEN];    //�豸���кţ�ֻ����
    DWORD        dwMajorScale;        //�������ţ�0�������ţ�1��������
    DWORD        dwMinorScale;        //�������ţ�0�������ţ�1��������
    BYTE        byRes[292];            //����
}NET_EHOME_DEVICE_CFG,*LPNET_EHOME_DEVICE_CFG;

#define    NET_EHOME_GET_DEVICE_INFO        1    //��ȡ�豸��Ϣ
#define    NET_EHOME_GET_VERSION_INFO        2    //��ȡ�汾��Ϣ
#define    NET_EHOME_GET_DEVICE_CFG        3    //��ȡ�豸��������
#define    NET_EHOME_SET_DEVICE_CFG        4    //�����豸��������

#define NET_EHOME_GET_GPS_CFG 20 //��ȡGPS����
#define NET_EHOME_SET_GPS_CFG 21 //����GPS����
#define NET_EHOME_GET_PIC_CFG 22 //��ȡOSD��ʾ����
#define NET_EHOME_SET_PIC_CFG 23 //����OSD��ʾ����
#define NET_EHOME_GET_WIRELESSINFO_CFG 24 //��ȡ����������Ϣ����
#define NET_EHOME_SET_WIRELESSINFO_CFG 25 //��������������Ϣ����

#define MAX_EHOME_PROTOCOL_LEN  1500

typedef struct tagNET_EHOME_REMOTE_CTRL_PARAM
{
    DWORD dwSize;
    void *lpCondBuffer;        //��������������
    DWORD  dwCondBufferSize;    //������������������
    void *lpInbuffer;          //���Ʋ���������
    DWORD  dwInBufferSize;      //���Ʋ�������������
    BYTE   byRes[32];
}NET_EHOME_REMOTE_CTRL_PARAM, *LPNET_EHOME_REMOTE_CTRL_PARAM;

//GPS����
typedef struct tagNET_EHOME_GPS_CFG
{
    DWORD        dwSize;
    DWORD        dwTransInterval;    //GPS�ϴ�ʱ�䣬��λ����
    DWORD        dwMaxSpeed;        //��������ֵ����λ������/Сʱ
    DWORD        dwMinSpeed;        //��������ֵ����λ������/Сʱ
    BYTE         bEnable;           //ʹ��,1-�����ã�2-����
    BYTE            byRes[63];                //����
}NET_EHOME_GPS_CFG, *LPNET_EHOME_GPS_CFG;

//OSD��ʾ����
typedef struct tagNET_EHOME_PIC_CFG
{
    DWORD        dwSize;
    BYTE            byChannelName[NAME_LEN];        //ͨ�����ƣ�GB2312���룬 V30�Ժ�汾ʹ��UTF-8���룩�����Ȳ�����31�ֽ�
    BOOL            bIsShowChanName;                            //�Ƿ���ʾͨ�����ƣ�0������ʾ��1����ʾ
    WORD            wChanNameXPos;                            //ͨ��������ʾX���꣬����704x576������
    WORD            wChanNameYPos;                            //ͨ��������ʾY���꣬����704x576�����ã�����ֵΪ16�ı���
    BOOL            bIsShowOSD;                                    //�Ƿ���ʾOSD��0������ʾ��1����ʾ
    WORD            wOSDXPos;                                        //OSD��ʾX����
    WORD            wOSDYPos;                                        //OSD��ʾY����
    BYTE            byOSDType;                                        /**OSD���� 0:XXXX-XX-XX(������) 1:XX-XX-XXXX(������) 2:XXXX��XX��XX��
                                                                    3:XX��XX��XXXX�� 4:XX-XX-XXXX(������) 5:XX��XX��XXXX��
                                                                    */
    BYTE            byOSDAtrib;                                    //OSD���� 0������ʾOSD 1��͸������˸ 2��͸��������˸ 3����˸����͸�� 4����͸��������˸
    BYTE            byRes1[2];                                            //����1
    BOOL            bIsShowWeek;                                    //�Ƿ���ʾ���ڣ�0������ʾ��1����ʾ
    BYTE            byRes2[64];                                        //����2
}NET_EHOME_PIC_CFG, *LPNET_EHOME_PIC_CFG;

typedef struct tagNET_EHOME_WIRELESS_INFO_CFG
{
    DWORD        dwSize;
    DWORD        dwInfoTransInterval;   //�ϴ����,��λ����
    BYTE         byEnable; //ʹ�ܣ�0-������ 1-����
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

//Ԥ������
typedef struct tagNET_EHOME_PREVIEWINFO_IN
{
    int iChannel;                        //ͨ���� 
    DWORD dwStreamType;                    // �������ͣ�0-��������1-������, 2-��������
    DWORD dwLinkMode;                        // 0��TCP��ʽ,1��UDP��ʽ,2: HRUDP��ʽ
    NET_EHOME_IPADDRESS struStreamSever;     //��ý���ַ
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

NET_DVR_API BOOL CALLBACK NET_ECMS_StartGetRealStream(LONG lUserID, LPNET_EHOME_PREVIEWINFO_IN pPreviewInfoIn, LPNET_EHOME_PREVIEWINFO_OUT pPreviewInfoOut ); //lUserID��SDK������û�ID�����豸ע��ص�ʱfDeviceRegisterCallBack����
NET_DVR_API BOOL CALLBACK NET_ECMS_StartGetRealStreamV11(LONG lUserID, LPNET_EHOME_PREVIEWINFO_IN_V11 pPreviewInfoIn, LPNET_EHOME_PREVIEWINFO_OUT pPreviewInfoOut);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopGetRealStream(LONG lUserID, LONG lSessionID);
NET_DVR_API BOOL CALLBACK NET_ECMS_StartPushRealStream(LONG lUserID, LPNET_EHOME_PUSHSTREAM_IN pPushInfoIn, LPNET_EHOME_PUSHSTREAM_OUT pPushInfoOut);

//��ѯ�ӿ�
#define MAX_FILE_NAME_LEN  100
#define LEN_32             32

typedef enum tagSEARCH_TYPE
{
    ENUM_SEARCH_TYPE_ERR = -1,
    ENUM_SEARCH_RECORD_FILE = 0,    //����¼���ļ�
    ENUM_SEARCH_PICTURE_FILE  = 1,  //����ͼƬ�ļ�
    ENUM_SEARCH_FLOW_INFO = 2,      //������ѯ
    ENUM_SEARCH_DEV_LOG = 3,        //�豸��־��ѯ
    ENUM_SEARCH_ALARM_HOST_LOG = 4, //����������־��ѯ
}SEARCH_TYPE_ENUM;

typedef enum
{
    ENUM_GET_NEXT_STATUS_SUCCESS = 1000,    //�ɹ���ȡ��һ�����ݣ������걾�����ݺ���Ҫ�ٴε���FindNext��ȡ��һ������
    ENUM_GET_NETX_STATUS_NO_FILE,           //û���ҵ�һ������
    ENUM_GET_NETX_STATUS_NEED_WAIT,         //���ݻ�δ��������ȴ�����������FindNext����
    ENUM_GET_NEXT_STATUS_FINISH,            //����ȫ��ȡ��
    ENUM_GET_NEXT_STATUS_FAILED,            //�����쳣
    ENUM_GET_NEXT_STATUS_NOT_SUPPORT        //�豸��֧�ָò�������֧�ֵĲ�ѯ����
}SEARCH_GET_NEXT_STATUS_ENUM;

//ʱ�����
typedef struct tagNET_EHOME_TIME
{
    WORD    wYear;      //��
    BYTE    byMonth;    //��
    BYTE    byDay;      //��
    BYTE    byHour;     //ʱ
    BYTE    byMinute;   //��
    BYTE    bySecond;   //��
    BYTE    byRes1;        
    WORD    wMSecond;   //����
    BYTE    byRes2[2];
}NET_EHOME_TIME, *LPNET_EHOME_TIME;

typedef struct tagNET_EHOME_FINDCOND
{
    DWORD               dwSize;
    LONG                iChannel;           //ͨ���ţ���1��ʼ
    DWORD               dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    NET_EHOME_TIME      struStartTime;      //��ʼʱ��
    NET_EHOME_TIME      struStopTime;       //����ʱ��
    SEARCH_TYPE_ENUM    enumSearchType;     //�������� 0-¼���ļ����ң���ӦstruRecordFileParam  1-ͼƬ�ļ����ң���ӦstruPicFileParam 2-������ѯ����ӦstruFlowParam
    union
    {
        BYTE            byLen[64];
        struct
        {
            DWORD       dwFileType;         /**�ļ����ͣ��ֽ�����������
                                            0xff-ȫ������¼�񣨲�����ͼƬ���ͣ� 
                                            0-��ʱ¼��
                                            1-�ƶ�����
                                            2-��������
                                            3-����|����
                                            4-����&����
                                            5-�����
                                            6-�ֶ�¼��
                                            7-�𶯱���
                                            8-��������
                                            9-���ܱ���������ȡ֤¼��
                                            10��0x0a��-PIR����
                                            11��0x0b��-���߱���
                                            12��0x0c��-���ȱ���
                                            13��0x0d��ȫ������
                                            100-ȫ������ͼƬ
                                            101-����ʶ��ͼƬ
                                            102-���鱨��ͼƬ
                                            103-�ֶ�ץ��ͼƬ
                                            104-�ط�ץ��ͼƬ
                                            **/

       }struRecordFileParam;
        struct
        {
            DWORD       dwFileType;         /*255��0xff��-ȫ�����ͣ�
                                            0��0x00��-��ʱץͼ
                                            1��0x01��-�ƶ����ץͼ
                                            2��0x02��-����ץͼ
                                            3��0x03��-����|�ƶ����ץͼ
                                            4��0x04��-����&�ƶ����ץͼ
                                            5��0x05��-�����ץͼ
                                            6��0x06��-�ֶ�ץͼ
                                            7��0x07��-�𶯱���ץͼ
                                            8��0x08��-������������ץͼ
                                            9��0x09��-���ܱ���ͼƬ
                                            10��0x0a��-PIR����ͼƬ
                                            11��0x0b��-���߱���ͼƬ
                                            12��0x0c��-���ȱ���ͼƬ
                                            13��0x0d��-�������ͼƬ
                                            14��0x0e��-Խ�����ͼƬ
                                            15��0x0f��-�����������ͼƬ
                                            16��0x10��-����������ͼƬ
                                            17��0x11��-�豸���ػط�ʱ��ͼ
                                            18��0x12��-�������ͼƬ
                                            19��0x13��-�����������ͼƬ
                                            20��0x14��-�뿪�������ͼƬ
                                            21��0x15��-�ǻ����ͼƬ
                                            22��0x16��-��Ա�ۼ����ͼƬ
                                            23��0x17��-�����˶����ͼƬ
                                            24��0x18��-ͣ�����ͼƬ
                                            25��0x19��-��Ʒ�������ͼƬ
                                            26��0x1a��-��Ʒ��ȡ���ͼƬ
                                            27��0x1b��-�������ͼƬ
                                            28��0x1c��-�ͻ����ϴ�ͼƬ
                                            */
        }struPicFileParam;
        struct
        {
            BYTE        bySearchMode;       //��ѯģʽ��1-��year��ѯ��2-��month��ѯ��3-��day��ѯ
        }struFlowParam;
    }unionSearchParam;
	BYTE                byStartIndex;       //��ѯ��ʼλ�ã������ϰ汾���°汾�в�����ʹ��
	BYTE                byRes1[1];
	DWORD               dwStartIndexEx;     //��չ��Ĳ�ѯ��ʼλ�ã� ���ӶԼ�¼���������֧��
	BYTE                byRes[122];
}NET_EHOME_FINDCOND, *LPNET_EHOME_FINDCOND;

//¼���ļ���������
typedef struct tagNET_EHOME_REC_FILE_COND
{
    DWORD           dwChannel;          //ͨ���ţ���1��ʼ
    DWORD           dwRecType;          /*0xff-ȫ������¼�񣨲�����ͼƬ���ͣ�
                                        0-��ʱ¼��
                                        1-�ƶ�����
                                        2-��������
                                        3-����|����
                                        4-����&����
                                        5-�����
                                        6-�ֶ�¼��
                                        7-�𶯱���
                                        8-��������
                                        9-���ܱ���������ȡ֤¼��
                                        10��0x0a��-PIR����
                                        11��0x0b��-���߱���
                                        12��0x0c��-���ȱ���
                                        13��0x0d��ȫ������
                                        100-ȫ������ͼƬ
                                        101-����ʶ��ͼƬ
                                        102-���鱨��ͼƬ
                                        103-�ֶ�ץ��ͼƬ
                                        104-�ط�ץ��ͼƬ
                                        */
    NET_EHOME_TIME  struStartTime;      //��ʼʱ��
    NET_EHOME_TIME  struStopTime;       //����ʱ��
    DWORD           dwStartIndex;       //��ѯ��ʼλ��
    DWORD           dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    BYTE            byLocalOrUTC;        //0-struStartTime��struStopTime�У���ʾ�����豸����ʱ�䣬���豸OSDʱ��  1-struStartTime��struStopTime�У���ʾ����UTCʱ��
    BYTE            byRes[63];
}NET_EHOME_REC_FILE_COND, *LPNET_EHOME_REC_FILE_COND;

//ͼƬ�ļ���������
typedef struct tagNET_EHOME_PIC_FILE_COND
{
    DWORD           dwChannel;          //ͨ���ţ���1��ʼ
    DWORD           dwPicType;          /*255��0xff��-ȫ������
                                        0��0x00��-��ʱץͼ
                                        1��0x01��-�ƶ����ץͼ
                                        2��0x02��-����ץͼ
                                        3��0x03��-����|�ƶ����ץͼ
                                        4��0x04��-����&�ƶ����ץͼ
                                        5��0x05��-�����ץͼ
                                        6��0x06��-�ֶ�ץͼ
                                        7��0x07��-�𶯱���ץͼ
                                        8��0x08��-������������ץͼ
                                        9��0x09��-���ܱ���ͼƬ
                                        10��0x0a��-PIR����ͼƬ
                                        11��0x0b��-���߱���ͼƬ
                                        12��0x0c��-���ȱ���ͼƬ
                                        13��0x0d��-�������ͼƬ
                                        14��0x0e��-Խ�����ͼƬ
                                        15��0x0f��-�����������ͼƬ
                                        16��0x10��-����������ͼƬ
                                        17��0x11��-�豸���ػط�ʱ��ͼ
                                        18��0x12��-�������ͼƬ
                                        19��0x13��-�����������ͼƬ
                                        20��0x14��-�뿪�������ͼƬ
                                        21��0x15��-�ǻ����ͼƬ
                                        22��0x16��-��Ա�ۼ����ͼƬ
                                        23��0x17��-�����˶����ͼƬ
                                        24��0x18��-ͣ�����ͼƬ
                                        25��0x19��-��Ʒ�������ͼƬ
                                        26��0x1a��-��Ʒ��ȡ���ͼƬ
                                        27��0x1b��-�������ͼƬ
                                        28��0x1c��-�ͻ����ϴ�ͼƬ
                                        */
    NET_EHOME_TIME  struStartTime;      //��ʼʱ��
    NET_EHOME_TIME  struStopTime;       //����ʱ��
    DWORD           dwStartIndex;       //��ѯ��ʼλ��
    DWORD           dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    BYTE            byLocalOrUTC;        //0-struStartTime��struStopTime�У���ʾ�����豸����ʱ�䣬���豸OSDʱ��  1-struStartTime��struStopTime�У���ʾ����UTCʱ��
    BYTE            byRes[63];
}NET_EHOME_PIC_FILE_COND, *LPNET_EHOME_PIC_FILE_COND;

//������ѯ����
typedef struct tagNET_EHOME_FLOW_COND
{
    BYTE            bySearchMode;       //��ѯģʽ��1-��year��ѯ��2-��month��ѯ��3-��day��ѯ
    BYTE            byRes[3];
    NET_EHOME_TIME  struStartTime;      //��ʼʱ��
    NET_EHOME_TIME  struStopTime;       //����ʱ��
    DWORD           dwStartIndex;       //��ѯ��ʼλ��
    DWORD           dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    BYTE            byLocalOrUTC;        //0-struStartTime��struStopTime�У���ʾ�����豸����ʱ�䣬���豸OSDʱ��  1-struStartTime��struStopTime�У���ʾ����UTCʱ��
    BYTE            byRes1[63];
}NET_EHOME_FLOW_COND, *LPNET_EHOME_FLOW_COND;

//�豸��־��ѯ����
typedef struct tagNET_EHOME_DEV_LOG_COND
{
    DWORD           dwMajorType;        //��־�����ͣ�1-������2-�쳣��3-������0xffff-ȫ��
    DWORD           dwMinorType;        //��־�����ͣ����豸���ߴ��붨����ͬ
    NET_EHOME_TIME  struStartTime;      //��ʼʱ��
    NET_EHOME_TIME  struStopTime;       //����ʱ��
    DWORD           dwStartIndex;       //��ѯ��ʼλ��
    DWORD           dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    BYTE            byLocalOrUTC;        //0-struStartTime��struStopTime�У���ʾ�����豸����ʱ�䣬���豸OSDʱ��  1-struStartTime��struStopTime�У���ʾ����UTCʱ��
    BYTE            byRes[63];
}NET_EHOME_DEV_LOG_COND, *LPNET_EHOME_DEV_LOG_COND;

//����������־��ѯ����
typedef struct tagNET_EHOME_ALARM_HOST_LOG_COND
{
    DWORD           dwMajorType;        //��־�����ͣ�1-������2-�쳣��3-������4-�¼���0xffff-ȫ��
    DWORD           dwMinorType;        //��־�����ͣ����豸���ߴ��붨����ͬ
    NET_EHOME_TIME  struStartTime;      //��ʼʱ��
    NET_EHOME_TIME  struStopTime;       //����ʱ��
    DWORD           dwStartIndex;       //��ѯ��ʼλ��
    DWORD           dwMaxFileCountPer;  //������������ļ�����������ļ���������Ҫȷ��ʵ�����绷��������������Ϊ8
    BYTE            byLocalOrUTC;        //0-struStartTime��struStopTime�У���ʾ�����豸����ʱ�䣬���豸OSDʱ��  1-struStartTime��struStopTime�У���ʾ����UTCʱ��
    BYTE            byRes1[63];
}NET_EHOME_ALARM_HOST_LOG_COND, *LPNET_EHOME_ALARM_HOST_LOG_COND;

//�ϰ��¼���ļ���Ϣ
typedef struct tagNET_EHOME_FINDDATA
{
    DWORD           dwSize;
    char            szFileName[MAX_FILE_NAME_LEN];  //�ļ���
    NET_EHOME_TIME  struStartTime;                  //�ļ��Ŀ�ʼʱ��
    NET_EHOME_TIME  struStopTime;                   //�ļ��Ľ���ʱ��
    DWORD           dwFileSize;                     //�ļ��Ĵ�С
    DWORD           dwFileMainType;                 //¼���ļ�������
    DWORD           dwFileSubType;                  //¼���ļ�������
    DWORD           dwFileIndex;                    //¼���ļ�����
    BYTE            byRes[128];
}NET_EHOME_FINDDATA, *LPNET_EHOME_FINDDATA;

//¼���ļ���Ϣ
typedef struct tagNET_EHOME_REC_FILE
{
    DWORD           dwSize;
    char            sFileName[MAX_FILE_NAME_LEN];   //�ļ���
    NET_EHOME_TIME  struStartTime;                  //�ļ��Ŀ�ʼʱ��
    NET_EHOME_TIME  struStopTime;                   //�ļ��Ľ���ʱ��
    DWORD           dwFileSize;                     //�ļ��Ĵ�С
    DWORD           dwFileMainType;                 //¼���ļ�������
    DWORD           dwFileSubType;                  //¼���ļ�������
    DWORD           dwFileIndex;                    //¼���ļ�����
    BYTE            byTimeDiffH;                    //struStartTime��struStopTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE            byTimeDiffM;                    //struStartTime��struStopTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE            byRes[126];
}NET_EHOME_REC_FILE, *LPNET_EHOME_REC_FILE;

//ͼƬ�ļ���Ϣ
typedef struct tagNET_EHOME_PIC_FILE
{
    DWORD           dwSize;
    char            sFileName[MAX_FILE_NAME_LEN];   //�ļ���
    NET_EHOME_TIME  struPicTime;                    //ͼƬ����ʱ��
    DWORD           dwFileSize;                     //�ļ��Ĵ�С
    DWORD           dwFileMainType;                 //ͼƬ�ļ�������
    DWORD           dwFileIndex;                    //ͼƬ�ļ�����
    BYTE            byTimeDiffH;                    //struPicTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE            byTimeDiffM;                    //struPicTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE            byRes[126];
}NET_EHOME_PIC_FILE, *LPNET_EHOME_PIC_FILE;

//������Ϣ
typedef struct tagNET_EHOME_FLOW_INFO
{
    DWORD   dwSize;
    DWORD   dwFlowValue;    //����ֵ
    DWORD   dwFlowIndex;    //��������
    BYTE    byRes[128];
}NET_EHOME_FLOW_INFO, *LPNET_EHOME_FLOW_INFO;

//�豸��־��Ϣ
#define MAX_LOG_INFO_LEN    8*1024   //��־������Ϣ����
typedef struct tagNET_EHOME_DEV_LOG
{
    NET_EHOME_TIME  struLogTime;                    //��־ʱ��
    DWORD           dwMajorType;                    //��־�����ͣ�1-������2-�쳣��3-����
    DWORD           dwMinorType;                    //��־�����ͣ����豸���ߴ��붨����ͬ
    DWORD           dwParamType;                    //�����Ͳ������ͣ��󲿷ֱ�ʾ�����ţ����û����ֵΪ0
    char            sLocalUser[NAME_LEN];           //�����û�
    char            sRemoteUser[NAME_LEN];          //Զ���û�
    char            sIPAddress[128];                //Զ������IP��ַ
    DWORD           dwChannelNo;                    //ͨ����
    DWORD           dwHardDiskNo;                   //Ӳ�̺�
    DWORD           dwAlarmInputChanNo;             //��������ͨ����
    DWORD           dwAlarmOutputChanNo;            //�������ͨ����
    char            sLogContext[MAX_LOG_INFO_LEN];  //��־����
    BYTE            byTimeDiffH;                    //struLogTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE            byTimeDiffM;                    //struLogTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE            byRes[62];
}NET_EHOME_DEV_LOG, *LPNET_EHOME_DEV_LOG;

//����������־��Ϣ
typedef struct tagNET_EHOME_ALARM_HOST_LOG
{
    NET_EHOME_TIME  struLogTime;                    //��־ʱ��
    DWORD           dwMajorType;                    //��־�����ͣ�1-������2-�쳣��3-������4-�¼�
    DWORD           dwMinorType;                    //��־�����ͣ����豸���ߴ��붨����ͬ
    DWORD           dwParamType;                    //�����Ͳ������ͣ��󲿷ֱ�ʾ�����ţ����û����ֵΪ0
    char            sUserName[NAME_LEN];            //�û���
    char            sIPAddress[128];                //Զ������IP��ַ
    char            sLogContext[MAX_LOG_INFO_LEN];  //��־����
    BYTE            byTimeDiffH;                    //struLogTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE            byTimeDiffM;                    //struLogTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE            byRes[62];
}NET_EHOME_ALARM_HOST_LOG, *LPNET_EHOME_ALARM_HOST_LOG;

NET_DVR_API LONG CALLBACK NET_ECMS_StartFindFile(LONG lUserID, LPNET_EHOME_FINDCOND pFindCond); //��֧��¼���ļ���ͼƬ�ļ���������ѯ
NET_DVR_API LONG CALLBACK NET_ECMS_FindNextFile(LONG lHandle, LPNET_EHOME_FINDDATA pFindData); //��֧��¼���ļ���ѯ

NET_DVR_API LONG CALLBACK NET_ECMS_StartFindFile_V11(LONG lUserID, LONG lSearchType, LPVOID pFindCond, DWORD dwCondSize);
NET_DVR_API LONG CALLBACK NET_ECMS_FindNextFile_V11(LONG lHandle, LPVOID pFindData, DWORD dwDataSize);
NET_DVR_API BOOL CALLBACK NET_ECMS_StopFindFile(LONG lHandle);


typedef struct tagNET_EHOME_PLAYBACK_INFO_IN
{
    DWORD       dwSize;
    DWORD       dwChannel;                    //�طŵ�ͨ����
    BYTE        byPlayBackMode;               //�ط�����ģʽ 0�������� 1����ʱ��
    BYTE        byStreamPackage;               //�ط��������ͣ��豸�˷�����������ʽ 0��PS��Ĭ�ϣ� 1��RTP
    BYTE        byRes[2];
    union
    {
        BYTE    byLen[512];
        struct
        {
            char   szFileName[MAX_FILE_NAME_LEN];          //�طŵ��ļ���
            DWORD  dwSeekType;                             //0-���ֽڳ��ȼ���ƫ����  1-��ʱ�䣨����������ƫ����
            DWORD  dwFileOffset;                           //�ļ�ƫ���������ĸ�λ�ÿ�ʼ���أ����dwSeekTypeΪ0��ƫ�������ֽڼ��㣬Ϊ1������������
            DWORD  dwFileSpan;                             //���ص��ļ���С��Ϊ0ʱ����ʾ����ֱ�����ļ�����Ϊֹ�����dwSeekTypeΪ0����С�����ֽڼ��㣬Ϊ1������������
        }struPlayBackbyName;
        struct 
        {
            NET_EHOME_TIME  struStartTime;            // ��ʱ��طŵĿ�ʼʱ��
            NET_EHOME_TIME  struStopTime;            // ��ʱ��طŵĽ���ʱ��
            BYTE    byLocalOrUTC;                     //0-�豸����ʱ�䣬���豸OSDʱ��  1-UTCʱ��
            BYTE    byDuplicateSegment;                //byLocalOrUTCΪ1ʱ��Ч 0-�ظ�ʱ��ε�ǰ�� 1-�ظ�ʱ��κ��
        }struPlayBackbyTime;
    }unionPlayBackMode;
    NET_EHOME_IPADDRESS struStreamSever;     //��ý���ַ
}NET_EHOME_PLAYBACK_INFO_IN, *LPNET_EHOME_PLAYBACK_INFO_IN;

typedef struct tagNET_EHOME_PLAYBACK_INFO_OUT
{
    LONG   lSessionID;     //ĿǰЭ�鲻֧�֣�����-1
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

//��̨��ؽṹ��
#define NET_EHOME_PTZ_CTRL                1000        //�ƾ�����
#define NET_EHOME_PRESET_CTRL        1001        //Ԥ�õ����
#define NET_EHOME_PZIN                        1002        //3D��ѡ�Ŵ���С
#define NET_EHOME_PTRACK                    1003        //3D�������

typedef enum
{
    PTZ_UP = 0,            //����
    PTZ_DOWN,            //����
    PTZ_LEFT,                //����
    PTZ_RIGHT,            //����
    PTZ_UPLEFT,            //������
    PTZ_DOWNLEFT,        //������
    PTZ_UPRIGHT,            //������
    PTZ_DOWNRIGHT,        //������
    PTZ_ZOOMIN,            //�佹��
    PTZ_ZOOMOUT,        //�佹��
    PTZ_FOCUSNEAR,        //�۽���
    PTZ_FOCUSFAR,        //�۽���
    PTZ_IRISSTARTUP,        //��Ȧ���
    PTZ_IRISSTOPDOWN,    //��Ȧ��С
    PTZ_LIGHT,            //�����
    PTZ_WIPER,            //��ˢ
    PTZ_AUTO            //�Զ�
}EN_PTZ_CMD;

typedef struct tagNET_EHOME_PTZ_PARAM
{
    DWORD dwSize;
    BYTE  byPTZCmd;        //��̨����μ�EN_PTZ_CMD
    BYTE  byAction;        //��̨������0-��ʼ��̨������1-ֹͣ��̨����
    BYTE  bySpeed;        //��̨�ٶȣ�0-7����ֵԽ���ٶ�Խ��
    BYTE  byRes[29];
}NET_EHOME_PTZ_PARAM, *LPNET_EHOME_PTZ_PARAM;

typedef struct tagNET_EHOME_PRESET_PARAM
{
    DWORD dwSize;
    BYTE  byPresetCmd;    //Ԥ�õ�������1-����Ԥ�õ㣬2-���Ԥ�õ㣬3-ת��Ԥ�õ�
    BYTE  byRes1[3];
    DWORD dwPresetIndex;    //Ԥ�õ���
    BYTE  byRes2[32];
}NET_EHOME_PRESET_PARAM, *LPNET_EHOME_PRESET_PARAM;

typedef struct tagNET_EHOME_PZIN_PARAM
{
    DWORD dwSize;
    BYTE  byAction;    //������0��С������->���£�����->���ϣ� 1�Ŵ�����->���£�����->���ϣ�
    BYTE  byRes1[3];
    NET_EHOME_ZONE struArea;    //��ѡ��Χ
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

#define MACADDR_LEN                    6       //mac��ַ����

typedef struct tagNET_EHOME_ETHERNET
{
    NET_EHOME_IPADDR    struDevIP;        //�豸IP��ַ
    NET_EHOME_IPADDR    struDevIPMask;    //�����ַ
    DWORD        dwNetInterface; // ��������1:10M��˫�� 2:10Mȫ˫�� 3:100M��˫�� 4:100Mȫ˫�� 6:1000Mȫ˫�� 5:10M/100M/1000M����Ӧ
    WORD        wDevPort;                    //�豸SDK�˿ںţ�8000
    WORD        wMTU;                        // MTU����
    BYTE        byMACAddr[MACADDR_LEN]; //mac��ַ
    BYTE        byRes[2];
}NET_EHOME_ETHERNET,*LPNET_EHOME_ETHERNET;
#define PASSWD_LEN                    16      //���볤��
typedef struct tagNET_EHOME_PPPOECFG
{
    DWORD        dwPPPoE;    //�Ƿ�����PPPOE��1����ʹ�ã�0����ʹ��
    char            sPPPoEUser[NAME_LEN];    //PPPoE�û���
    char            sPPPoEPassword[PASSWD_LEN];    //PPPoE����
    NET_EHOME_IPADDR    struPPPoEIP;        //PPPoE IP��ַ
}NET_EHOME_PPPOECFG,*LPNET_EHOME_PPPOECFG;

typedef struct tagNET_EHOME_NETWORK_CFG
{
    DWORD        dwSize;                //�ṹ���С
    NET_EHOME_ETHERNET        struEtherNet;    //��̫����
    NET_EHOME_IPADDR        struGateWayIP;//���ص�ַ
    NET_EHOME_IPADDR        struMultiCastIP;//�ಥ��ַ
    NET_EHOME_IPADDR        struDDNSServer1IP;//DDNS1������IP
    NET_EHOME_IPADDR        struDDNSServer2IP;//DDNS2������IP
    NET_EHOME_IPADDR        struAlarmHostIP;    //��������IP
    WORD        wAlarmHostPort;    //���������˿ں�
    WORD        wIPResolverPort;    //�����������˿�
    NET_EHOME_IPADDR        struIPResolver; //������������ַ
    NET_EHOME_PPPOECFG        struPPPoE;    //PPPoE����
    WORD        wHTTPPort;;        //Http�˿�
    BYTE        byRes[674];        //����
}NET_EHOME_NETWORK_CFG,*LPNET_EHOME_NETWORK_CFG;

typedef struct tagNET_EHOME_COMPRESSION_COND
{
    DWORD        dwSize;    //�ṹ���С
    DWORD        dwChannelNum;    //ͨ���ţ���1��ʼ
    BYTE        byCompressionType;//�������ͣ�1-��������2-��������3-������
    BYTE        byRes[23];
}NET_EHOME_COMPRESSION_COND,*LPNET_EHOME_COMPRESSION_COND;

typedef struct tagNET_EHOME_COMPRESSION_CFG
{
    DWORD        dwSize;    //�ṹ���С
    BYTE        byStreamType;    //��������
    BYTE        byPicQuality;    //ͼ������0:��� 1:�ϸ� 2:�е� 3:�� 4:�ϵ� 5:���
    BYTE        byBitRateType;    //��������0:������ 1:������
    BYTE        byRes1;    //����
    DWORD        dwResolution;    //�ֱ��ʣ�0:DCIF 1:CIF 2:QCIF 3:4CIF 4:2CIF 6:QVGA��320x240�� 16:VGA 17:UXGA 18:SVGA 19:HD720p 20:hd900 21:XVGA    22:SXGAp(1360*1024)
    //27:1080P(1920*1080)    28:2560x1920 /*500W*/    29:1600x304    30:2048x1536 /*300W*/
    //31:2448x2048/*500W*/        32:2448x1200        33:2448x800    34:XGA/*(1024*768)*/
    //35:SXGA/*(1280*1024)*/    36:WD1/*(960*576/960*480)*/    37:HD1080I    38-WXGA(1440*900)��
    //39-HD_F(1920*1080/1280*720)��40-HD_H(1920*540/1280*360)��  41-HD_Q(960*540/630*360)��  
    //42-2336*1744��    43-1920*1456��44-2592*2048��    45-3296*2472��46-1376*768��47-1366*768,                 
    //48-1360*768,  49-WSXGA+��50-720*720��51-1280*1280��52-2048*768��53-2048*2048
    //54-2560x2048,  55-3072x2048 ,  56-2304*1296  57-WXGA(1280*800),  58-1600*600    
    //59-2592*1944  60-2752*2208,    61-384*288,    62-4000*3000,  63-4096*2160,  64-3840*2160,
    //65-4000*2250, 66-3072*1728,
    DWORD        dwVideoBitRate;    //0-32K 1-48k 2-64K 3-80K 4-96K 5-128K 6-160k 7-192K 8-224K 9-256K 10-320K 11-384K 12-448K 13-512K 14-640K 15-768K 16-896K 17-1024K 18-1280K 19-1536K 20-1792K 21-2048K 22-�Զ���
    DWORD        dwMaxBitRate;    //�Զ�������
    DWORD        dwVideoFrameRate;    //��Ƶ֡�ʣ�4�ֽ�������0:ȫ֡�� 1:1/16 2:1/8 3:1/4 4:1/2 5:1 6:2 7:4 8:6 9:8 10:10 11:12 12:16 13:20 14:15 15:18 16:22
    WORD        wIntervalFrameI;    // I֡�������Χ1~400
    BYTE        byIntervalBPFrame; //֡���� 0:BBP 1��BP 2:P
    BYTE        byRes[41];    //����
}NET_EHOME_COMPRESSION_CFG,*LPNET_EHOME_COMPRESSION_CFG;

#define    MAX_TIME_SEGMENT        8   //ʱ���,ehomeЭ��ֻ֧��4��
#define MAX_ANALOG_ALARMOUT     32      //���32·ģ�ⱨ�����
#define MAX_ANALOG_CHANNUM      32      //���32��ģ��ͨ��
#define MAX_DIGIT_CHANNUM    480/*512-32*/      //���480������ͨ��,������Ᵽ��

typedef struct tagNET_EHOME_ALARM_TIME_COND
{
    DWORD        dwSize;    //�ṹ���С
    BYTE        byAlarmType;    //�������ͣ�0-�ƶ���⣬1-��Ƶ��ʧ��2-�ڵ�������3-�������룬4-���������9-������
    BYTE        byWeekday;    //0-Mon��1-Tues��2-Wed��3-Thur��4-Fri��5-Sat��6-Sun
    BYTE        byRes1[2];
    DWORD        dwChannel;    //ͨ���ţ���1��ʼ
    BYTE        byRes2[20];
}NET_EHOME_ALARM_TIME_COND,*LPNET_EHOME_ALARM_TIME_COND;

typedef struct tagNET_EHOME_SCHEDTIME
{
    BYTE        byStartHour;    //��ʼʱ�䣺ʱ
    BYTE        byStartMin;    //��ʼʱ�䣺��
    BYTE        byStopHour;    //����ʱ�䣺ʱ
    BYTE        byStopMin;    //����ʱ�䣺��
}NET_EHOME_SCHEDTIME,*LPNET_EHOME_SCHEDTIME;

typedef struct tagNET_EHOME_ALARM_TIME_CFG
{
    DWORD        dwSize;    //�ṹ���С
    NET_EHOME_SCHEDTIME    struSchedTime[MAX_TIME_SEGMENT];//����ʱ���
    BYTE        bySchedTimeCount;    //ֻ��������ʱ�����
    BYTE        byRes[43];
}NET_EHOME_ALARM_TIME_CFG,*LPNET_EHOME_ALARM_TIME_CFG;

typedef struct tagNET_EHOME_ALARMOUT_CFG
{
    DWORD        dwSize;    //�ṹ���С
    BYTE        sAlarmOutName[NAME_LEN];    //�����������
    WORD        wAlarmOutDelay;    //�����ʱ��0��5�룻1��10�룻 2��30�룻 3��1�֣� 4��2�֣� 5��5�֣� 6��10�֣� 7�����
    BYTE        byRes[26];    //����
}NET_EHOME_ALARMOUT_CFG,*LPNET_EHOME_ALARMOUT_CFG;

typedef struct tagNET_EHOME_ALARMOUT_STATUS_CFG
{
    DWORD        dwSize;    //�ṹ���С
    BYTE        byAlarmOutStatus;    //1-�������������0-�رձ������
    BYTE        byRes[11];
}NET_EHOME_ALARMOUT_STATUS_CFG,*LPNET_EHOME_ALARMOUT_STATUS_CFG;

typedef struct tagNET_EHOME_ALARMIN_COND
{
    DWORD        dwSize;    //�ṹ���С
    DWORD        dwAlarmInNum;    //������ţ���1��ʼ
    DWORD        dwPTZChan;// PTZ������Ƶͨ�����,��1��ʼ
    BYTE        byRes[20];
}NET_EHOME_ALARMIN_COND,*LPNET_EHOME_ALARMIN_COND;

typedef struct tagNET_EHOME_LINKAGE_ALARMOUT
{
    DWORD        dwAnalogAlarmOutNum;    //ֻ����ģ�ⱨ������
    BYTE        byAnalogAlarmOut[MAX_ANALOG_ALARMOUT];    //ģ�ⱨ�������0����ʹ�ã�1��ʹ��
    BYTE        byRes[5000];    //������Э������û��IP��������������ӿ���λ��������
}NET_EHOME_LINKAGE_ALARMOUT,*LPNET_EHOME_LINKAGE_ALARMOUT;

typedef struct tagNET_EHOME_LINKAGE_PTZ
{
    BYTE    byUsePreset;    //�Ƿ����Ԥ�õ㣬0����ʹ�ã�1��ʹ��
    BYTE    byUseCurise;    //�Ƿ����Ѳ����0����ʹ�ã�1��ʹ��
    BYTE    byUseTrack;    //�Ƿ���ù켣��0����ʹ�ã�1��ʹ��
    BYTE    byRes1;        //����
    WORD    wPresetNo;    //Ԥ�õ�ţ���Χ��1~256��Э���й涨��1��256��ʵ�������豸֧��300
    WORD    wCuriseNo;    //Ѳ��·���ţ���Χ��1~16
    WORD    wTrackNo;    //�켣�ţ���Χ��1~16
    BYTE    byRes2[6];    //����
}NET_EHOME_LINKAGE_PTZ,*LPNET_EHOME_LINKAGE_PTZ;

typedef struct tagNET_EHOME_ALARMIN_LINKAGE_TYPE
{
    BYTE    byMonitorAlarm;    //�������Ͼ��棬0����ʹ�ã�1��ʹ��
    BYTE    bySoundAlarm;    //����������0����ʹ�ã�1��ʹ��
    BYTE    byUpload;        //�ϴ����ģ�0����ʹ�ã�1��ʹ��
    BYTE    byAlarmout;        //�������������0����ʹ�ã�1��ʹ��-
    BYTE    byEmail;            //�ʼ�������0����ʹ�ã�1��ʹ��
    BYTE    byRes1[3];        //����
    NET_EHOME_LINKAGE_PTZ    struPTZLinkage;    //PTZ����
    NET_EHOME_LINKAGE_ALARMOUT    struAlarmOut;    //�����������
    BYTE    byRes[128];
}NET_EHOME_ALARMIN_LINKAGE_TYPE,*LPNET_EHOME_ALARMIN_LINKAGE_TYPE;

typedef    struct    tagNET_EHOME_RECORD_CHAN
{
    BYTE    byAnalogChanNum;    //ֻ����ģ��ͨ����
    BYTE    byAnalogChan[MAX_ANALOG_CHANNUM];    //ģ��ͨ����0����ʹ�ã�1��ʹ��
    BYTE    byRes1[3];    //����
    WORD    wDigitChanNum;    //ֻ��������ͨ����
    BYTE    byDigitChan[MAX_DIGIT_CHANNUM];    //����ͨ����0����ʹ�ã�1��ʹ��
    BYTE    byRes2[62];    //����
}NET_EHOME_RECORD_CHAN,*LPNET_EHOME_RECORD_CHAN;

typedef struct tagNET_EHOME_ALARMIN_CFG
{
    DWORD        dwSize;    //�ṹ���С
    BYTE        sAlarmInName[NAME_LEN];    //������������
    BYTE        byAlarmInType;    //���������ͣ�0��������1������
    BYTE        byUseAlarmIn;    //�Ƿ���0����ʹ�ã�1��ʹ��
    BYTE        byRes1[2];            //����
    NET_EHOME_ALARMIN_LINKAGE_TYPE    struLinkageType;    //����ģʽ
    NET_EHOME_RECORD_CHAN    struRecordChan;    //����¼��ͨ��
    BYTE        byRes2[128];        //����
}NET_EHOME_ALARMIN_CFG,*LPNET_EHOME_ALARMIN_CFG;

typedef    struct    tagNET_EHOME_MANUAL_IOOUT_CTRL
{
    DWORD        dwSize;    //�ṹ���С
    DWORD        dwChan;        //IO�����ţ���1��ʼ
    DWORD        dwDelayTime;        //�����������ʱ�䣨�룩��ֵΪ0��ʾһֱ���������ָ��ʱ�䳤�Ƚ��������
    BYTE        byAction;        //0-�رձ������,1-�����������
    BYTE        byRes[19];
}NET_EHOME_MANUAL_IOOUT_CTRL,*LPNET_EHOME_MANUAL_IOOUT_CTRL;

typedef struct tagNET_EHOME_IMAGE_CFG
{
    DWORD       dwSize;    //�ṹ���С
    BYTE        byHue;    //ɫ����0��255
    BYTE        byContrast;    //�Աȶ�,0~255
    BYTE        byBright;        //���ȣ�0��255
    BYTE        bySaturation;    //���Ͷ�
    BYTE        byRes[24];
}NET_EHOME_IMAGE_CFG,*LPNET_EHOME_IMAGE_CFG;


typedef struct tagBINARY_DATA_INFO
{
    DWORD dwMagic;
    DWORD dwCommand;  //������
    DWORD dwSequence; //���
    DWORD dwStatus;   //״ֵ̬
    DWORD dwBufLen;
    BYTE  byMsgType;    /*�������ͣ�0-��Ч��1-�����ģ�2-Ӧ����*/
    char  sSerialNumber[12];//�豸���кţ���δʹ��
    //char  sDeviceID[16];//�豸ID
    BYTE  byVersion;
    BYTE  byRes[2];
} BINARY_DATA_INFO, *LPBINARY_DATA_INFO;


#define    NET_EHOME_GET_NETWORK_CFG        5    //��ȡ�������
#define    NET_EHOME_SET_NETWORK_CFG        6    //�����������
#define    NET_EHOME_GET_COMPRESSION_CFG    7    //��ȡѹ������
#define    NET_EHOME_SET_COMPRESSION_CFG    8    //����ѹ������
#define    NET_EHOME_GET_IMAGE_CFG            9    //��ȡͼ�����
#define    NET_EHOME_SET_IMAGE_CFG            10    //����ͼ�����
#define    NET_EHOME_GET_ALARMIN_CFG        11        //��ȡ�����������
#define    NET_EHOME_SET_ALARMIN_CFG        12    //���ñ����������
#define    NET_EHOME_GET_ALARM_TIME_CFG    13    //��ȡ��������ʱ�����
#define    NET_EHOME_SET_ALARM_TIME_CFG    14    //���ñ�������ʱ�����
#define    NET_EHOME_GET_ALARMOUT_CFG        15    //��ȡ�����������
#define    NET_EHOME_SET_ALARMOUT_CFG        16    //���ñ����������
#define    NET_EHOME_GET_ALARMOUT_STATUS_CFG        17    //��ȡ�������״̬����
#define    NET_EHOME_SET_ALARMOUT_STATUS_CFG        18    //���ñ������״̬����
#define    NET_EHOME_MANUAL_IOOUT            19    //�ֶ�����IO���

typedef enum tagNET_CMS_ENUM_PROXY_TYPE
{
	ENUM_PROXY_TYPE_NETSDK = 0,	//NetSDK����
	ENUM_PROXY_TYPE_HTTP					//HTTP����
}NET_CMS_ENUM_PROXY_TYPE;

typedef struct tagNET_EHOME_PT_PARAM
{
	NET_EHOME_IPADDRESS struIP;   //������IP�Ͷ˿�
	BYTE	byProtocolType;							//Э�����ͣ�0-TCP
	BYTE	byProxyType;									//�������ͣ�0-NetSDK����1-HTTP����
	BYTE	byRes[2];
}NET_EHOME_PT_PARAM, *LPNET_EHOME_Proxy_PARAM;

typedef struct tagNET_EHOME_PASSTHROUGH_PARAM
{
	DWORD	dwSequence;	//�������
	DWORD	dwUUID;		//�ỰID
	BYTE			byRes[64];
}NET_EHOME_PASSTHROUGH_PARAM, *LPNET_EHOME_PASSTHROUGH_PARAM;

typedef struct tagNET_EHOME_PTXML_PARAM
{
    void*   pRequestUrl;        //����URL
    DWORD   dwRequestUrlLen;    //����URL����
    void*   pCondBuffer;        //������������XML��ʽ���ݣ�
    DWORD   dwCondSize;         //������������С
    void*   pInBuffer;          //���뻺������XML��ʽ���ݣ�
    DWORD   dwInSize;           //���뻺������С
    void*   pOutBuffer;         //�����������XML��ʽ���ݣ�
    DWORD   dwOutSize;          //�����������С
    DWORD   dwReturnedXMLLen;   //ʵ�ʴ��豸���յ���XML���ݵĳ���
    BYTE    byRes[32];          //����
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
    void* lpInbuffer;          //���Ʋ���������
    DWORD  dwInBufferSize;      //���Ʋ�������������
    DWORD  dwSendTimeOut;  //���ݷ��ͳ�ʱʱ��,��λms��Ĭ��5s
    DWORD  dwRecvTimeOut;  //���ݽ��ճ�ʱʱ��,��λms��Ĭ��5s
    void *lpOutBuffer;     //���������
    DWORD dwOutBufferSize;  //�����������С
    void *lpStatusBuffer;   //״̬������,������Ҫ����ΪNULL
    DWORD dwStatusBufferSize;  //״̬��������С
    BYTE   byRes[16];
}NET_EHOME_XML_REMOTE_CTRL_PARAM, *LPNET_EHOME_XML_REMOTE_CTRL_PARAM;
NET_DVR_API BOOL CALLBACK NET_ECMS_XMLRemoteControl(LONG lUserID, LPNET_EHOME_XML_REMOTE_CTRL_PARAM lpCtrlParam, DWORD dwCtrlSize);


#define    EHOME_CMSALARM_EXCEPTION      0x105  //CMS���ձ����쳣
NET_DVR_API BOOL CALLBACK NET_ECMS_SetExceptionCallBack(DWORD dwMessage, HANDLE hWnd, void (CALLBACK* fExceptionCallBack)(DWORD dwType, LONG iUserID, LONG iHandle, void* pUser), void* pUser);

NET_DVR_API BOOL CALLBACK NET_ECMS_TranBuf(LONG lUserID, DWORD dwLength, void *pBuf);


typedef enum
{
    LONG_CFG_CREATED,  //�����ӽ����ɹ�
    LONG_CFG_CREATE_FAIL,   //�����ӽ���ʧ��
    LONG_CFG_DATA,  //��������ͨ����
    LONG_CFG_TERMINATE  //������������Ϣ
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
