#ifndef _HC_EHOME_ALARM_H_
#define _HC_EHOME_ALARM_H_

#include "HCEHomePublic.h"

//����
#define   EHOME_ALARM_UNKNOWN 0  //δ֪��������
#define   EHOME_ALARM                    1        //Ehome��������
#define   EHOME_ALARM_HEATMAP_REPORT    2        //�ȶ�ͼ����
#define   EHOME_ALARM_FACESNAP_REPORT    3        //����ץ�ı���
#define   EHOME_ALARM_GPS                4        //GPS��Ϣ�ϴ�
#define   EHOME_ALARM_CID_REPORT        5        //��������CID�澯�ϴ�
#define   EHOME_ALARM_NOTICE_PICURL    6        //ͼƬURL�ϱ�
#define   EHOME_ALARM_NOTIFY_FAIL        7        //�첽ʧ��֪ͨ
#define   EHOME_ALARM_SELFDEFINE         9      //�Զ��屨���ϴ�
#define   EHOME_ALARM_DEVICE_NETSWITCH_REPORT 10    //�豸�����л��ϴ�
#define   EHOME_ALARM_ACS               11       //�Ž��¼��ϱ�
#define EHOME_ALARM_WIRELESS_INFO   12   //����������Ϣ�ϴ�

#define MAX_TIME_LEN                32      //ʱ���ַ�������
#define MAX_REMARK_LEN        64      //������ע����
#define MAX_URL_LEN                512     //URL����
#define CID_DES_LEN                    32      //CID������������

typedef enum
{
    ALARM_TYPE_DISK_FULL = 0,                     //0-Ӳ����
    ALARM_TYPE_DISK_WRERROR,                     //1-��дӲ�̳���
    ALARM_TYPE_VIDEO_LOST = 5,                   //5-��Ƶ(�ź�)��ʧ*/
    ALARM_TYPE_EXTERNAL,                         //6-�ⲿ(�ź���)����
    ALARM_TYPE_VIDEO_COVERED,                    //7-��Ƶ�ڸ�
    ALARM_TYPE_MOTION,                           //8-�ƶ����
    ALARM_TYPE_STANDARD_NOTMATCH,                //9-��ʽ��ƥ��
    ALARM_TYPE_SPEEDLIMIT_EXCEED,                //10-����
    ALARM_TYPE_PIR,                                 //11-PIR����
    ALARM_TYPE_WIRELESS,                         //12-���߱���
    ALARM_TYPE_CALL_HELP,                         //13-���ȱ���
    ALARM_TYPE_DISARM,                             //14-����������
    ALARM_TYPE_STREAM_PRIVATE,                     //15-������˽״̬�ı�֪ͨ
    ALARM_TYPE_PIC_UPLOAD_FAIL,                     //16-�豸�ϴ�ͼƬʧ��
    ALARM_TYPE_LOCAL_REC_EXCEPTION,                 //17-�豸����¼��ץͼ���쳣
    ALARM_TYPE_UPGRADE_FAIL,                     //18-�豸�汾����ʧ��
    ALARM_TYPE_ILLEGAL_ACCESS,                     //19-�Ƿ�����
    ALARM_TYPE_SOUNDLIMIT_EXCEED = 80,           //80-�����ֱ�������
    ALARM_TYPE_TRIFFIC_VIOLATION = 90,             //90-Υ�±���
    ALARM_TYPE_ALARM_CONTROL,                     //91-���ر���
    ALARM_TYPE_FACE_DETECTION = 97,                 //97-�������
    ALARM_TYPE_DEFOUSE_DETECTION,                 //98-�齹���
    ALARM_TYPE_AUDIO_EXCEPTION,                     //99-��Ƶ�쳣���
    ALARM_TYPE_SCENE_CHANGE,                     //100-�����������
    ALARM_TYPE_TRAVERSE_PLANE,                     //101-��Խ�����棨Խ����⣩
    ALARM_TYPE_ENTER_AREA,                         //102-Ŀ���������
    ALARM_TYPE_LEAVE_AREA,                         //103-Ŀ���뿪����
    ALARM_TYPE_INTRUSION,                         //104-�ܽ����֣��������֣�
    ALARM_TYPE_LOITER,                             //105-�ǻ�
    ALARM_TYPE_LEFT_TAKE,                         //106-��Ʒ������ȡ
    ALARM_TYPE_CAR_STOP,                         //107-ͣ��
    ALARM_TYPE_MOVE_FAST,                         //108-�����˶�
    ALARM_TYPE_HIGH_DENSITY,                     //109-��Ա�ۼ�
    ALARM_TYPE_PDC_BY_TIME,                         //110-��������ʱ���ͳ���ϱ�
    ALARM_TYPE_PDC_BY_FRAME,                     //111-��������֡ͳ���ϱ�
    ALARM_TYPE_LEFT,                             //112-��Ʒ����
    ALARM_TYPE_TAKE,                             //113-��Ʒ��ȡ
    ALARM_TYPE_ROLLOVER,                         //114-�෭
    ALARM_TYPE_COLLISION,                       //115-��ײ

    ALARM_TYPE_FLOW_OVERRUN =256,                    //256-�������ޱ���
    ALARM_TYPE_WARN_FLOW_OVERRUN,              //257-������������


    ALARM_TYPE_DEV_CHANGED_STATUS = 700,           //700-�豸״̬�ı䱨���ϴ�
    ALARM_TYPE_CHAN_CHANGED_STATUS,               //701-ͨ��״̬�ı䱨���ϱ�
    ALARM_TYPE_HD_CHANGED_STATUS,               //702-����״̬�ı䱨���ϱ�
    ALARM_TYPE_DEV_TIMING_STATUS,               //703-��ʱ�ϱ��豸״̬��Ϣ
    ALARM_TYPE_CHAN_TIMING_STATUS,               //704-��ʱ�ϱ�ͨ��״̬��Ϣ
    ALARM_TYPE_HD_TIMING_STATUS,               //705-��ʱ�ϱ�����״̬��Ϣ
    ALARM_TYPE_RECORD_ABNORMAL,               //706-¼���쳣����ǰʱ��㱾��Ӧ������ִ��¼��ģ������Ϊ�쳣ԭ�����豸�޷�����¼��

    ALARM_TYPE_ENV_LIMIT = 8800,                 //8800-�������������ޱ���
    ALARM_TYPE_ENV_REAL_TIME,                     //8801-����������ʵʱ�����ϱ�
    ALARM_TYPE_ENV_EXCEPTION,                     //8802-�����쳣�ϱ�
    ALARM_TYPE_HIGH_TEMP = 40961,                 //40961-�豸�¶ȹ��߱���
    ALARM_TYPE_ACC_EXCEPTION,                     //40962-�豸���ٶ��쳣����
}EN_ALARM_TYPE;

/*
dwAlarmType                           dwAlarmAction            dwVideoChannel        dwAlarmInChannel            dwDiskNumber        byRemark
ALARM_TYPE_DISK_FULL                  0-������ʼ 1-��������    N/A                  N/A                            Ӳ�̺�                N/A
ALARM_TYPE_DISK_WRERROR               0-������ʼ 1-��������    N/A                    N/A                            Ӳ�̺�                N/A
ALARM_TYPE_VIDEO_LOST                 0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_EXTERNAL                   0-������ʼ 1-��������    N/A                    ���������                    N/A                    N/A
ALARM_TYPE_VIDEO_COVERED              0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_MOTION                     0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_STANDARD_NOTMATCH          0-������ʼ 1-��������    N/A                    N/A                            N/A                    N/A
ALARM_TYPE_SPEEDLIMIT_EXCEED          0-������ʼ 1-��������    ����ֵ                ��ǰ�ٶ�                    N/A                    N/A
ALARM_TYPE_PIR                        0-������ʼ 1-��������    ���������            N/A                            N/A                    N/A
ALARM_TYPE_WIRELESS                   0-������ʼ 1-��������    N/A                    ���߱��������                N/A                    N/A
ALARM_TYPE_CALL_HELP                  0-������ʼ 1-��������    ���������            N/A                            N/A                    N/A
ALARM_TYPE_DISARM                     0-���� 1-����            N/A                    N/A                            N/A                    N/A
ALARM_TYPE_STREAM_PRIVATE             0-���� 1-�ر�            N/A                    N/A                            N/A                    N/A
ALARM_TYPE_PIC_UPLOAD_FAIL            0-�����쳣�򱨾�         ͨ����                ��������ץ���ϴ�ʧ��ʱ        �ֶ�ץ���ϴ�ʧ��ʱ    N/A                
��Ӧ�ı�������                �ֶ�ץ����ˮ��        N/A
ALARM_TYPE_LOCAL_REC_EXCEPTION        0-�����쳣�򱨾�         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_UPGRADE_FAIL               0-�����쳣�򱨾�         N/A                    N/A                            N/A                    ��ǰ�汾�ţ���ʽ��Vx.y.z buildYYMMDD
ALARM_TYPE_ILLEGAL_ACCESS             0-�����쳣�򱨾�         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_SOUNDLIMIT_EXCEED          0-�����쳣�򱨾�         ������ֵ�ֱ�*10        ��ǰ����ֵ�ֱ�*10            N/A                    N/A
ALARM_TYPE_TRIFFIC_VIOLATION          0-�����쳣�򱨾�         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_ALARM_CONTROL              0-�����쳣�򱨾�         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_FACE_DETECTION             0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_DEFOUSE_DETECTION          0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_AUDIO_EXCEPTION            0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_SCENE_CHANGE               0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_TRAVERSE_PLANE             0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_ENTER_AREA                 0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_LEAVE_AREA                 0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_INTRUSION                  0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_LOITER                     0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_LEFT_TAKE                  0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_CAR_STOP                   0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_MOVE_FAST                  0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_HIGH_DENSITY               0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_PDC_BY_TIME                0-�����ϱ�               ͨ����                ��������                    �뿪����            ������ͳ�ƽ���ʱ�䣨YYYY-MM-DD HHMISS��
ALARM_TYPE_PDC_BY_FRAME               0-�����ϱ�               ͨ����                ��������                    �뿪����            N/A
ALARM_TYPE_LEFT                       0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_TAKE                       0-������ʼ 1-��������    ͨ����                N/A                            N/A                    N/A
ALARM_TYPE_ENV_LIMIT                  0-�����ϱ�               ���(Ĭ��104)        N/A                            N/A                    �������쳣������Ϣ��������256�ֽ�
ALARM_TYPE_ENV_REAL_TIME              0-�����ϱ�               ���(Ĭ��104)        N/A                            N/A                    ���������ʵʱ��ֵ��double���ͣ�����11.20��
ALARM_TYPE_ENV_EXCEPTION              0-�����ϱ�               N/A                    N/A                            N/A                    �������쳣������Ϣ��������256�ֽ�
ALARM_TYPE_HIGH_TEMP                  0-�����ϱ�               N/A                    N/A                            N/A                    N/A
ALARM_TYPE_ACC_EXCEPTION              0-�����ϱ�               N/A                    N/A                            N/A                    N/A
*/

//�豸״̬�ı��ϱ�
typedef struct tagNET_EHOME_DEV_STATUS_CHANGED
{
    BYTE   byDeviceStatus; //�豸״̬��0��������1��CPUռ����̫�ߣ�����85%,2��Ӳ���������紮���쳣
    BYTE   byRes[11];       //���� 
}NET_EHOME_DEV_STATUS_CHANGED, *LPNET_EHOME_DEV_STATUS_CHANGED;


//ͨ��״̬�ı��ϱ�
typedef struct tagNET_EHOME_CHAN_STATUS_CHANGED
{
    WORD  wChanNO;      //ͨ���� 
    BYTE   byChanStatus;     //ͨ��״̬����λ��ʾ 
    //bit0������״̬��0-����/ɾ����1-����/���
    //ģ��ͨ���ɽ��õ����ã��������õ����ã��ϱ����ֶ�
    //����ͨ����ӵ�ɾ��������ɾ����������ӣ��ϱ����ֶ�
    //bit1������״̬��0-�����ߣ�1-����
    //bit2���ź�״̬��0-���źţ�1-���ź�
    //bit3��¼��״̬��0-����¼�� 1-��¼��
    //bit4��IPͨ����Ϣ�ı�״̬��0-δ�ı� 1-�иı䣬��λ��ʾ��ͨ������
    //����Ϣ�����˸ı䣬������ӵ�IPC�й�������֪ͨ�ϲ���������� 
    BYTE   byRes[9];
}NET_EHOME_CHAN_STATUS_CHANGED, *LPNET_EHOME_CHAN_STATUS_CHANGED;

//����״̬�ı�
typedef struct tagNET_EHOME_HD_STATUS_CHANGED
{
    DWORD       dwVolume;       //Ӳ����������λ��MB
    WORD        wHDNo;      //Ӳ�̺�
    BYTE        byHDStatus;     //Ӳ�̵�״̬, 0-�1-����,2-�쳣,3-����Ӳ�̳���,
    //4-δ��ʽ��, 5-δ����״̬(����Ӳ��),6-Ӳ�����ڸ�ʽ��
    BYTE        byRes[5];       //���� 
}NET_EHOME_HD_STATUS_CHANGED, *LPNET_EHOME_HD_STATUS_CHANGED;

//��ʱ�ϱ��豸״̬��
typedef struct tagNET_EHOME_DEV_TIMING_STATUS
{
    DWORD       dwMemoryTotal; //�ڴ���������λKbyte
    DWORD       dwMemoryUsage;     //�ڴ�ʹ��������λKbyte
    BYTE        byCPUUsage;         //CPUʹ���ʣ�0-100
    BYTE        byMainFrameTemp;    //�����¶ȣ���λ�����϶�
    BYTE        byBackPanelTemp;        //�����¶ȣ���λ�����϶�
    BYTE        byRes;
}NET_EHOME_DEV_TIMING_STATUS, *LPNET_EHOME_DEV_TIMING_STATUS;

//��ʱ�ϱ�ͨ��״̬��-����ͨ��
typedef struct tagNET_EHOME_CHAN_TIMING_STATUS_SINGLE
{
    DWORD dwBitRate;       //ʵ�����ʣ���λkbps 
    WORD  wChanNO;    //ͨ���� 
    BYTE   byLinkNum;  //�ͻ������ӵĸ���
    BYTE   byRes[5];
}NET_EHOME_CHAN_TIMING_STATUS_SINGLE, *LPNET_EHOME_CHAN_TIMING_STATUS_SINGLE;

//��ʱ�ϱ�����״̬��
typedef struct tagNET_EHOME_HD_TIMING_STATUS_SINGLE
{
    DWORD   dwHDFreeSpace;    //Ӳ��ʣ��ռ䣬��λ��MB 
    WORD    wHDNo;  //���̺� 
    BYTE    byRes[6];
}NET_EHOME_HD_TIMING_STATUS_SINGLE, *LPNET_EHOME_HD_TIMING_STATUS_SINGLE;


typedef union tagNET_EHOME_ALARM_STATUS_UNION
{
    BYTE    byRes[12]; //�������С
    NET_EHOME_DEV_STATUS_CHANGED struDevStatusChanged;
    NET_EHOME_CHAN_STATUS_CHANGED struChanStatusChanged;
    NET_EHOME_HD_STATUS_CHANGED struHdStatusChanged;
    NET_EHOME_DEV_TIMING_STATUS struDevTimeStatus;
    NET_EHOME_CHAN_TIMING_STATUS_SINGLE struChanTimeStatus;
    NET_EHOME_HD_TIMING_STATUS_SINGLE struHdTimeStatus;
}NET_EHOME_ALARM_STATUS_UNION, *LPNET_EHOME_ALARM_STATUS_UNION;

typedef struct tagNET_EHOME_ALARM_INFO
{
    DWORD   dwSize;
    char    szAlarmTime[MAX_TIME_LEN];    //��������ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    char    szDeviceID[MAX_DEVICE_ID_LEN];//�豸ע��ID
    DWORD   dwAlarmType;                  //�������ͼ�EN_ALARM_TYPEö�ٱ���
    DWORD   dwAlarmAction;                //��������0:��ʼ    1:ֹͣ
    DWORD   dwVideoChannel;               //�������е������ע��
    DWORD   dwAlarmInChannel;             //�������е������ע��
    DWORD   dwDiskNumber;                 //�������е������ע��
    BYTE    byRemark[MAX_REMARK_LEN];     //�������е������ע��
    BYTE    byRetransFlag;                //�ش���ǣ�0-ʵʱ����1-�ش���
    BYTE    byTimeDiffH;  //szAlarmTime��szAlarmUploadTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE    byTimeDiffM;  //szAlarmTime��szAlarmUploadTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE    byRes1;
    char    szAlarmUploadTime[MAX_TIME_LEN];    //�����ϴ�ʱ�䣨�豸����ʱ�䣩��ʱ���ʽ��YYYY-MM-DD HH:MM:SS
    NET_EHOME_ALARM_STATUS_UNION    uStatusUnion;
    BYTE    byRes2[16];
}NET_EHOME_ALARM_INFO, *LPNET_EHOME_ALARM_INFO;

typedef struct tagNET_EHOME_GPS_INFO
{
    DWORD    dwSize;
    char    bySampleTime[MAX_TIME_LEN];      //GPS����ʱ�䣬�豸����ʱ�䣬��ʽ��YYYY-MM-DD HH:MM:SS
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //�豸ע��ID
    char    byDivision[2];                   //division[0]:'E'or'W'(����/����), division[1]:'N'or'S'(��γ/��γ)
    BYTE    bySatelites;                     //��������
    BYTE    byPrecision;                     //�������ӣ�ԭʼֵ*100
    DWORD    dwLongitude;                     //���ȣ�ȡֵ��ΧΪ��0��180*3600*100����ת����ʽΪ��longitude= ʵ�ʶ�*3600*100+ʵ�ʷ�*60*100+ʵ����*100
    DWORD    dwLatitude;                      //γ�ȣ�ȡֵ��ΧΪ��0��90*3600*100����ת����ʽΪ��latitude = ʵ�ʶ�*3600*100+ʵ�ʷ�*60*100+ʵ����*100
    DWORD    dwDirection;                     //����ȡֵ��ΧΪ��0��359.9*100������������Ϊ0��ת����ʽΪ��direction= ʵ�ʷ���*100
    DWORD    dwSpeed;                         //�ٶȣ�ȡֵ��ΧΪ��0��999.9*100000����ת����ʽΪ��speed =ʵ���ٶ�*100000���൱��cm/h
    DWORD    dwHeight;                         //�߶ȣ���λ��cm
    BYTE    byRetransFlag;                     //�ش���ǣ�0-ʵʱ����1-�ش���
    BYTE    byLocateMode;                      //��λģʽ(��ֵ0)����NMEA0183 3.00�汾�����ֵ��0-������λ��1- ��֣�2- ���㣬3- ������Ч 
    BYTE    byTimeDiffH;  //bySampleTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE    byTimeDiffM;  //bySampleTimee����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    DWORD   dwMileage;                       //�豸�����ͳ�ƣ���λΪ�ף����ͣ�ÿ���һ�ο�������00:00:00ʱ���㣬�����ۼӵ���������ϱ������� 
    BYTE    byRes[56];
}NET_EHOME_GPS_INFO, *LPNET_EHOME_GPS_INFO;

typedef struct tagNET_EHOME_ALARMWIRELESSINFO
{
    BYTE     byDeviceID[MAX_DEVICE_ID_LEN];   //�豸ID
    DWORD       dwDataTraffic;    /* ��������,��λ��MB,ʵ������*100*/
    BYTE      bySignalIntensity;  /* �ź�ǿ�ȣ�0~100*/
    BYTE      byRes[127];
} NET_EHOME_ALARMWIRELESSINFO, *LPNET_EHOME_ALARMWIRELESSINFO;


typedef struct tagNET_EHOME_HEATMAP_VALUE
{
    DWORD dwMaxHeatMapValue;    //����ȶ�ֵ
    DWORD dwMinHeatMapValue;    //��С�ȶ�ֵ
    DWORD dwTimeHeatMapValue;    //ƽ���ȶ�ֵ
}NET_EHOME_HEATMAP_VALUE, *LPNET_EHOME_HEATMAP_VALUE;

typedef struct tagNET_EHOME_PIXEL_ARRAY_SIZE
{
    DWORD dwLineValue;    //���ص���ֵ
    DWORD dwColumnValue;//���ص���ֵ
}NET_EHOME_PIXEL_ARRAY_SIZE, *LPNET_EHOME_PIXEL_ARRAY_SIZE;

typedef struct tagNET_EHOME_HEATMAP_REPORT
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //�豸ע��ID
    DWORD   dwVideoChannel;                     //ͨ����
    char    byStartTime[MAX_TIME_LEN];       //��ʼʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    char    byStopTime[MAX_TIME_LEN];        //����ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    NET_EHOME_HEATMAP_VALUE struHeatmapValue;//�ȶ�ֵ
    NET_EHOME_PIXEL_ARRAY_SIZE struPixelArraySize;    //�ȶ�ͼ��С
    char    byPixelArrayData[MAX_URL_LEN];     //�ȶ�ͼ��������
    BYTE    byRetransFlag;                     //�ش���ǣ�0-ʵʱ����1-�ش���
    BYTE    byTimeDiffH; //byStartTime��byStopTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE    byTimeDiffM;  //byStartTime��byStopTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE    byRes[61];
}NET_EHOME_HEATMAP_REPORT, *LPNET_EHOME_HEATMAP_REPORT;

typedef struct tagNET_EHOME_HUMAN_FEATURE
{
    BYTE byAgeGroup;    //�������ԣ�1-Ӥ�׶���2-��ͯ��3-���꣬4-�����꣬5-���꣬6-׳�꣬7-���꣬8-�����꣬9-����
    BYTE bySex;            //�Ա����ԣ�1-�У�2-Ů
    BYTE byEyeGlass;    //�Ƿ���۾���1-������2-��
    BYTE byRes[13];
}NET_EHOME_HUMAN_FEATURE, *LPNET_EHOME_HUMAN_FEATURE;

typedef struct tagNET_EHOME_FACESNAP_REPORT
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //�豸ע��ID
    DWORD   dwVideoChannel;                     //ͨ����
    char    byAlarmTime[MAX_TIME_LEN];       //����ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    DWORD   dwFacePicID;                     //����ͼID
    DWORD   dwFaceScore;                     //�������֣�0-100
    DWORD   dwTargetID;                         //Ŀ��ID
    NET_EHOME_ZONE    struTarketZone;             //Ŀ�����򣬹�һ�����꣺��ֵΪ��ǰ����İٷֱȴ�С*1000������ΪС�������λ
    NET_EHOME_ZONE  struFacePicZone;         //������ͼ���򣬹�һ�����꣺��ֵΪ��ǰ����İٷֱȴ�С*1000������ΪС�������λ
    NET_EHOME_HUMAN_FEATURE struHumanFeature;//������
    DWORD    dwStayDuration;                     //ͣ��������ʱ��
    DWORD    dwFacePicLen;                     //����ͼ���ȣ���λ���ֽ�
    char    byFacePicUrl[MAX_URL_LEN];         //������ͼ��������
    DWORD    dwBackgroudPicLen;                     //����ͼƬ���ȣ���λ���ֽ�
    char    byBackgroudPicUrl[MAX_URL_LEN];         //����ͼƬ��������
    BYTE    byRetransFlag;                     //�ش���ǣ�0-ʵʱ����1-�ش���
    BYTE    byTimeDiffH;  //byAlamTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
    BYTE    byTimeDiffM;  //byAlamTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE    byRes[61];
}NET_EHOME_FACESNAP_REPORT, *LPNET_EHOME_FACESNAP_REPORT;

typedef struct tagNET_EHOME_CID_PARAM
{
    DWORD dwUserType;    //�û����ͣ�1�����û� 2�����û�������ֵ��ʾ��Ч
    LONG  lUserNo;        //�û����ͣ�-1��ʾ��Ч
    LONG  lZoneNo;        //�����ţ�-1��ʾ��Ч
    LONG  lKeyboardNo;    //���̺ţ�
    LONG  lVideoChanNo;    //��Ƶͨ����
    LONG  lDiskNo;        //Ӳ�̺�
    LONG  lModuleAddr;    //ģ���ַ
    char  byUserName[NAME_LEN];    //�û���
    BYTE  byRes[32];
}NET_EHOME_CID_PARAM, *LPNET_EHOME_CID_PARAM;

typedef struct tagNET_EHOME_CID_INFO
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];		//�豸ע��ID
    DWORD   dwCIDCode;							//CID�������
    DWORD   dwCIDType;							//CID��������
    DWORD   dwSubSysNo;                         //�����������ϵͳ�ţ�0Ϊȫ�ֱ��棬��ϵͳ��Χ0~32
    char    byCIDDescribe[CID_DES_LEN];         //CID��������
    char    byTriggerTime[MAX_TIME_LEN];		//CID��������ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    char    byUploadTime[MAX_TIME_LEN];         //CID�����ϴ�ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    NET_EHOME_CID_PARAM struCIDParam;			//CID��������
	BYTE 	byTimeDiffH;						//byTriggerTime��byUploadTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
	BYTE 	byTimeDiffM;						//byTriggerTime��byUploadTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE    byRes[62];
}NET_EHOME_CID_INFO, *LPNET_EHOME_CID_INFO;

typedef struct tagNET_EHOME_NOTICE_PICURL
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //�豸ע��ID
    WORD    wPicType;    //ͼƬҵ�����ͣ�0-PU��ʱץͼ�ϴ����ģ�1-PU����ץͼ�ϴ�����(AlarmType��AlarmChan��AlarmTime �����ڵ���PicTypeΪʱ����Ч) ��2-CU�ֶ�ץͼ�ϴ����ģ�
    //3-CU����ͼƬ��4-�ⲿ����PUץͼ�ϴ����ģ�����DVRͨ�����ڽ��յ�POS��ˢ������ץͼ����
    WORD    wAlarmType;					//�������ͣ�6-�������룬7-��Ƶ�ڵ���8-�ƶ���⣬11-PIR������12-�Ŵű�����13-���ȱ���    
    DWORD   dwAlarmChan;				//����ͨ�����
    char    byAlarmTime[MAX_TIME_LEN];  //��������ʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    DWORD   dwCaptureChan;				//ͼƬץ��ͨ�����
    char    byPicTime[MAX_TIME_LEN];    //ͼƬʱ�䣨�豸����ʱ�䣩����ʽ��YYYY-MM-DD HH:MM:SS
    char    byPicUrl[MAX_URL_LEN];      //ͼƬ��������
    DWORD   dwManualSnapSeq;			//������ˮ�ţ��ֽ���������PicTypeֵΪ2��3ʱ��Ч
    BYTE    byRetransFlag;              //�ش���ǣ�0-ʵʱ����-�ش���
	BYTE 	byTimeDiffH;				//byAlarmTime��byPicTime����ʱ�׼ʱ�䣨UTC����ʱ�Сʱ����-12 ... +14,0xff��ʾ��Ч
	BYTE 	byTimeDiffM;				//byAlarmTime��byPicTime����ʱ�׼ʱ�䣨UTC����ʱ����ӣ���-30,0, 30, 45, 0xff��ʾ��Ч
    BYTE    byRes[29];
}NET_EHOME_NOTICE_PICURL, *LPNET_EHOME_NOTICE_PICURL;

typedef struct tagNET_EHOME_NOTIFY_FAIL_INFO
{
    DWORD dwSize;
    char  byDeviceID[MAX_DEVICE_ID_LEN];	//�豸ע��ID
    WORD  wFailedCommand;					//ʧ�ܵ����1-ץͼʧ��
    WORD  wPicType;							//ͼƬҵ�����ͣ�ʧ������Ϊץͼʧ��ʱ��Ч��0-PU��ʱץͼ�ϴ����ģ�1-PU����ץͼ�ϴ����ģ�2-CU�ֶ�ץͼ�ϴ����ģ�
    //3-CU����ͼƬ��4-�ⲿ����PUץͼ�ϴ�����    
    DWORD    dwManualSnapSeq;				//������ˮ�ţ��ֽ���������PicTypeֵΪ2��3ʱ��Ч
    BYTE    byRetransFlag;                  //�ش���ǣ�0-ʵʱ����1-�ش���    
    BYTE    byRes[31];
}NET_EHOME_NOTIFY_FAIL_INFO, *LPNET_EHOME_NOTIFY_FAIL_INFO;

typedef struct tagNET_EHOME_ALARM_MSG
{
    DWORD dwAlarmType;      //�������ͣ���EN_ALARM_TYPE
    void *pAlarmInfo;       //�������ݣ��ṹ�壩
    DWORD dwAlarmInfoLen;   //�ṹ�屨�����ݳ���
    void *pXmlBuf;          //�������ݣ�XML��
    DWORD dwXmlBufLen;      //xml�������ݳ���
    char  sSerialNumber[NET_EHOME_SERIAL_LEN]; //�豸���кţ����ڽ���Token��֤
    BYTE  byRes[20];
}NET_EHOME_ALARM_MSG, *LPNET_EHOME_ALARM_MSG;

typedef BOOL (CALLBACK *EHomeMsgCallBack)(LONG iHandle, NET_EHOME_ALARM_MSG *pAlarmMsg, void *pUser);

typedef struct tagNET_EHOME_ALARM_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struAddress;  //���ؼ�����Ϣ��IPΪ0.0.0.0������£�Ĭ��Ϊ���ص�ַ���������������£�Ĭ��Ϊ�Ӳ���ϵͳ��ȡ���ĵ�һ��
    EHomeMsgCallBack    fnMsgCb; //������Ϣ�ص�����
    void * pUserData;   //�û�����
    BYTE  byProtocolType;  //Э�����ͣ�0-TCP,1-UDP
    BYTE  byUseCmsPort; //�Ƿ���CMS�˿�,0-�����ã���0-���ã��������cms�˿ڣ�Э�������ֶ���Ч����ʱ���ؼ�����ϢstruAddress��ػػ���ַ��
    BYTE  byUseThreadPool;  //0-�ص�����ʱ��ʹ���̳߳أ�1-�ص�����ʱ����ʹ���̳߳أ�Ĭ������£������ص���ʱ��ʹ���̳߳�
    BYTE  byRes[29];
}NET_EHOME_ALARM_LISTEN_PARAM, *LPNET_EHOME_ALARM_LISTEN_PARAM;


//��ʼ��������ʼ��
NET_DVR_API BOOL  CALLBACK NET_EALARM_Init();
NET_DVR_API BOOL  CALLBACK NET_EALARM_Fini();

//��ȡ������
NET_DVR_API DWORD CALLBACK NET_EALARM_GetLastError();

//��־
NET_DVR_API BOOL CALLBACK NET_EALARM_SetLogToFile(LONG iLogLevel, const char *strLogDir, BOOL bAutoDel);

//��ȡ�汾��
NET_DVR_API DWORD CALLBACK NET_EALARM_GetBuildVersion();

NET_DVR_API LONG CALLBACK NET_EALARM_StartListen(NET_EHOME_ALARM_LISTEN_PARAM *pAlarmListenParam);

NET_DVR_API BOOL CALLBACK NET_EALARM_StopListen(LONG iListenHandle);


NET_DVR_API BOOL  CALLBACK NET_EALARM_SetSDKLocalCfg(  NET_EHOME_LOCAL_CFG_TYPE enumType, void* const lpInBuff);
NET_DVR_API BOOL  CALLBACK NET_EALARM_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType,void *lpOutBuff);



#endif //_HC_EHOME_ALARM_H_