#ifndef _HC_EHOME_ALARM_H_
#define _HC_EHOME_ALARM_H_

#include "HCEHomePublic.h"

//报警
#define   EHOME_ALARM_UNKNOWN 0  //未知报警类型
#define   EHOME_ALARM                    1        //Ehome基本报警
#define   EHOME_ALARM_HEATMAP_REPORT    2        //热度图报告
#define   EHOME_ALARM_FACESNAP_REPORT    3        //人脸抓拍报告
#define   EHOME_ALARM_GPS                4        //GPS信息上传
#define   EHOME_ALARM_CID_REPORT        5        //报警主机CID告警上传
#define   EHOME_ALARM_NOTICE_PICURL    6        //图片URL上报
#define   EHOME_ALARM_NOTIFY_FAIL        7        //异步失败通知
#define   EHOME_ALARM_SELFDEFINE         9      //自定义报警上传
#define   EHOME_ALARM_DEVICE_NETSWITCH_REPORT 10    //设备网络切换上传
#define   EHOME_ALARM_ACS               11       //门禁事件上报
#define EHOME_ALARM_WIRELESS_INFO   12   //无线网络信息上传

#define MAX_TIME_LEN                32      //时间字符串长度
#define MAX_REMARK_LEN        64      //报警备注长度
#define MAX_URL_LEN                512     //URL长度
#define CID_DES_LEN                    32      //CID报警描述长度

typedef enum
{
    ALARM_TYPE_DISK_FULL = 0,                     //0-硬盘满
    ALARM_TYPE_DISK_WRERROR,                     //1-读写硬盘出错
    ALARM_TYPE_VIDEO_LOST = 5,                   //5-视频(信号)丢失*/
    ALARM_TYPE_EXTERNAL,                         //6-外部(信号量)报警
    ALARM_TYPE_VIDEO_COVERED,                    //7-视频遮盖
    ALARM_TYPE_MOTION,                           //8-移动侦测
    ALARM_TYPE_STANDARD_NOTMATCH,                //9-制式不匹配
    ALARM_TYPE_SPEEDLIMIT_EXCEED,                //10-超速
    ALARM_TYPE_PIR,                                 //11-PIR报警
    ALARM_TYPE_WIRELESS,                         //12-无线报警
    ALARM_TYPE_CALL_HELP,                         //13-呼救报警
    ALARM_TYPE_DISARM,                             //14-布撤防报警
    ALARM_TYPE_STREAM_PRIVATE,                     //15-码流隐私状态改变通知
    ALARM_TYPE_PIC_UPLOAD_FAIL,                     //16-设备上传图片失败
    ALARM_TYPE_LOCAL_REC_EXCEPTION,                 //17-设备本地录像（抓图）异常
    ALARM_TYPE_UPGRADE_FAIL,                     //18-设备版本升级失败
    ALARM_TYPE_ILLEGAL_ACCESS,                     //19-非法访问
    ALARM_TYPE_SOUNDLIMIT_EXCEED = 80,           //80-声音分贝数超标
    ALARM_TYPE_TRIFFIC_VIOLATION = 90,             //90-违章报警
    ALARM_TYPE_ALARM_CONTROL,                     //91-布控报警
    ALARM_TYPE_FACE_DETECTION = 97,                 //97-人脸侦测
    ALARM_TYPE_DEFOUSE_DETECTION,                 //98-虚焦侦测
    ALARM_TYPE_AUDIO_EXCEPTION,                     //99-音频异常侦测
    ALARM_TYPE_SCENE_CHANGE,                     //100-场景变更报警
    ALARM_TYPE_TRAVERSE_PLANE,                     //101-穿越警戒面（越界侦测）
    ALARM_TYPE_ENTER_AREA,                         //102-目标进入区域
    ALARM_TYPE_LEAVE_AREA,                         //103-目标离开区域
    ALARM_TYPE_INTRUSION,                         //104-周界入侵（区域入侵）
    ALARM_TYPE_LOITER,                             //105-徘徊
    ALARM_TYPE_LEFT_TAKE,                         //106-物品遗留拿取
    ALARM_TYPE_CAR_STOP,                         //107-停车
    ALARM_TYPE_MOVE_FAST,                         //108-快速运动
    ALARM_TYPE_HIGH_DENSITY,                     //109-人员聚集
    ALARM_TYPE_PDC_BY_TIME,                         //110-人流量按时间段统计上报
    ALARM_TYPE_PDC_BY_FRAME,                     //111-人流量单帧统计上报
    ALARM_TYPE_LEFT,                             //112-物品遗留
    ALARM_TYPE_TAKE,                             //113-物品拿取
    ALARM_TYPE_ROLLOVER,                         //114-侧翻
    ALARM_TYPE_COLLISION,                       //115-碰撞

    ALARM_TYPE_FLOW_OVERRUN =256,                    //256-流量超限报警
    ALARM_TYPE_WARN_FLOW_OVERRUN,              //257-流量超限提醒


    ALARM_TYPE_DEV_CHANGED_STATUS = 700,           //700-设备状态改变报警上传
    ALARM_TYPE_CHAN_CHANGED_STATUS,               //701-通道状态改变报警上报
    ALARM_TYPE_HD_CHANGED_STATUS,               //702-磁盘状态改变报警上报
    ALARM_TYPE_DEV_TIMING_STATUS,               //703-定时上报设备状态信息
    ALARM_TYPE_CHAN_TIMING_STATUS,               //704-定时上报通道状态信息
    ALARM_TYPE_HD_TIMING_STATUS,               //705-定时上报磁盘状态信息
    ALARM_TYPE_RECORD_ABNORMAL,               //706-录像异常，当前时间点本来应该是在执行录像的，结果因为异常原因导致设备无法正常录像。

    ALARM_TYPE_ENV_LIMIT = 8800,                 //8800-动环环境量超限报警
    ALARM_TYPE_ENV_REAL_TIME,                     //8801-动环环境量实时数据上报
    ALARM_TYPE_ENV_EXCEPTION,                     //8802-动环异常上报
    ALARM_TYPE_HIGH_TEMP = 40961,                 //40961-设备温度过高报警
    ALARM_TYPE_ACC_EXCEPTION,                     //40962-设备加速度异常报警
}EN_ALARM_TYPE;

/*
dwAlarmType                           dwAlarmAction            dwVideoChannel        dwAlarmInChannel            dwDiskNumber        byRemark
ALARM_TYPE_DISK_FULL                  0-报警开始 1-报警结束    N/A                  N/A                            硬盘号                N/A
ALARM_TYPE_DISK_WRERROR               0-报警开始 1-报警结束    N/A                    N/A                            硬盘号                N/A
ALARM_TYPE_VIDEO_LOST                 0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_EXTERNAL                   0-报警开始 1-报警结束    N/A                    报警输入号                    N/A                    N/A
ALARM_TYPE_VIDEO_COVERED              0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_MOTION                     0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_STANDARD_NOTMATCH          0-报警开始 1-报警结束    N/A                    N/A                            N/A                    N/A
ALARM_TYPE_SPEEDLIMIT_EXCEED          0-报警开始 1-报警结束    限速值                当前速度                    N/A                    N/A
ALARM_TYPE_PIR                        0-报警开始 1-报警结束    报警器编号            N/A                            N/A                    N/A
ALARM_TYPE_WIRELESS                   0-报警开始 1-报警结束    N/A                    无线报警器编号                N/A                    N/A
ALARM_TYPE_CALL_HELP                  0-报警开始 1-报警结束    报警器编号            N/A                            N/A                    N/A
ALARM_TYPE_DISARM                     0-布防 1-撤防            N/A                    N/A                            N/A                    N/A
ALARM_TYPE_STREAM_PRIVATE             0-开启 1-关闭            N/A                    N/A                            N/A                    N/A
ALARM_TYPE_PIC_UPLOAD_FAIL            0-发生异常或报警         通道号                报警联动抓拍上传失败时        手动抓拍上传失败时    N/A                
对应的报警类型                手动抓拍流水号        N/A
ALARM_TYPE_LOCAL_REC_EXCEPTION        0-发生异常或报警         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_UPGRADE_FAIL               0-发生异常或报警         N/A                    N/A                            N/A                    当前版本号，格式：Vx.y.z buildYYMMDD
ALARM_TYPE_ILLEGAL_ACCESS             0-发生异常或报警         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_SOUNDLIMIT_EXCEED          0-发生异常或报警         噪声阈值分贝*10        当前噪声值分贝*10            N/A                    N/A
ALARM_TYPE_TRIFFIC_VIOLATION          0-发生异常或报警         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_ALARM_CONTROL              0-发生异常或报警         N/A                    N/A                            N/A                    N/A
ALARM_TYPE_FACE_DETECTION             0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_DEFOUSE_DETECTION          0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_AUDIO_EXCEPTION            0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_SCENE_CHANGE               0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_TRAVERSE_PLANE             0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_ENTER_AREA                 0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_LEAVE_AREA                 0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_INTRUSION                  0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_LOITER                     0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_LEFT_TAKE                  0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_CAR_STOP                   0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_MOVE_FAST                  0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_HIGH_DENSITY               0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_PDC_BY_TIME                0-报警上报               通道号                进入人数                    离开人数            人流量统计结束时间（YYYY-MM-DD HHMISS）
ALARM_TYPE_PDC_BY_FRAME               0-报警上报               通道号                进入人数                    离开人数            N/A
ALARM_TYPE_LEFT                       0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_TAKE                       0-报警开始 1-报警结束    通道号                N/A                            N/A                    N/A
ALARM_TYPE_ENV_LIMIT                  0-报警上报               点号(默认104)        N/A                            N/A                    报警、异常描述信息，不超过256字节
ALARM_TYPE_ENV_REAL_TIME              0-报警上报               点号(默认104)        N/A                            N/A                    动环相关量实时数值（double类型，比如11.20）
ALARM_TYPE_ENV_EXCEPTION              0-报警上报               N/A                    N/A                            N/A                    报警、异常描述信息，不超过256字节
ALARM_TYPE_HIGH_TEMP                  0-报警上报               N/A                    N/A                            N/A                    N/A
ALARM_TYPE_ACC_EXCEPTION              0-报警上报               N/A                    N/A                            N/A                    N/A
*/

//设备状态改变上报
typedef struct tagNET_EHOME_DEV_STATUS_CHANGED
{
    BYTE   byDeviceStatus; //设备状态，0－正常；1－CPU占用率太高，超过85%,2－硬件错误，例如串口异常
    BYTE   byRes[11];       //保留 
}NET_EHOME_DEV_STATUS_CHANGED, *LPNET_EHOME_DEV_STATUS_CHANGED;


//通道状态改变上报
typedef struct tagNET_EHOME_CHAN_STATUS_CHANGED
{
    WORD  wChanNO;      //通道号 
    BYTE   byChanStatus;     //通道状态，按位表示 
    //bit0：启用状态，0-禁用/删除，1-启用/添加
    //模拟通道由禁用到启用，或者启用到禁用，上报该字段
    //数字通道添加到删除，或者删除到重新添加，上报该字段
    //bit1：在线状态，0-不在线，1-在线
    //bit2：信号状态，0-无信号，1-有信号
    //bit3：录像状态，0-不在录像 1-在录像
    //bit4：IP通道信息改变状态，0-未改变 1-有改变，这位表示该通道的配
    //置信息发生了改变，比如添加的IPC有过更换，通知上层更新能力集 
    BYTE   byRes[9];
}NET_EHOME_CHAN_STATUS_CHANGED, *LPNET_EHOME_CHAN_STATUS_CHANGED;

//磁盘状态改变
typedef struct tagNET_EHOME_HD_STATUS_CHANGED
{
    DWORD       dwVolume;       //硬盘容量，单位：MB
    WORD        wHDNo;      //硬盘号
    BYTE        byHDStatus;     //硬盘的状态, 0-活动1-休眠,2-异常,3-休眠硬盘出错,
    //4-未格式化, 5-未连接状态(网络硬盘),6-硬盘正在格式化
    BYTE        byRes[5];       //保留 
}NET_EHOME_HD_STATUS_CHANGED, *LPNET_EHOME_HD_STATUS_CHANGED;

//定时上报设备状态项
typedef struct tagNET_EHOME_DEV_TIMING_STATUS
{
    DWORD       dwMemoryTotal; //内存总量，单位Kbyte
    DWORD       dwMemoryUsage;     //内存使用量，单位Kbyte
    BYTE        byCPUUsage;         //CPU使用率，0-100
    BYTE        byMainFrameTemp;    //机箱温度，单位：摄氏度
    BYTE        byBackPanelTemp;        //背板温度，单位：摄氏度
    BYTE        byRes;
}NET_EHOME_DEV_TIMING_STATUS, *LPNET_EHOME_DEV_TIMING_STATUS;

//定时上报通道状态项-单个通道
typedef struct tagNET_EHOME_CHAN_TIMING_STATUS_SINGLE
{
    DWORD dwBitRate;       //实际码率，单位kbps 
    WORD  wChanNO;    //通道号 
    BYTE   byLinkNum;  //客户端连接的个数
    BYTE   byRes[5];
}NET_EHOME_CHAN_TIMING_STATUS_SINGLE, *LPNET_EHOME_CHAN_TIMING_STATUS_SINGLE;

//定时上报磁盘状态项
typedef struct tagNET_EHOME_HD_TIMING_STATUS_SINGLE
{
    DWORD   dwHDFreeSpace;    //硬盘剩余空间，单位：MB 
    WORD    wHDNo;  //磁盘号 
    BYTE    byRes[6];
}NET_EHOME_HD_TIMING_STATUS_SINGLE, *LPNET_EHOME_HD_TIMING_STATUS_SINGLE;


typedef union tagNET_EHOME_ALARM_STATUS_UNION
{
    BYTE    byRes[12]; //联合体大小
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
    char    szAlarmTime[MAX_TIME_LEN];    //报警触发时间（设备本地时间），格式，YYYY-MM-DD HH:MM:SS
    char    szDeviceID[MAX_DEVICE_ID_LEN];//设备注册ID
    DWORD   dwAlarmType;                  //报警类型见EN_ALARM_TYPE枚举变量
    DWORD   dwAlarmAction;                //报警动作0:开始    1:停止
    DWORD   dwVideoChannel;               //各报警中的意义见注释
    DWORD   dwAlarmInChannel;             //各报警中的意义见注释
    DWORD   dwDiskNumber;                 //各报警中的意义见注释
    BYTE    byRemark[MAX_REMARK_LEN];     //各报警中的意义见注释
    BYTE    byRetransFlag;                //重传标记，0-实时包，1-重传包
    BYTE    byTimeDiffH;  //szAlarmTime，szAlarmUploadTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE    byTimeDiffM;  //szAlarmTime，szAlarmUploadTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE    byRes1;
    char    szAlarmUploadTime[MAX_TIME_LEN];    //报警上传时间（设备本地时间），时间格式，YYYY-MM-DD HH:MM:SS
    NET_EHOME_ALARM_STATUS_UNION    uStatusUnion;
    BYTE    byRes2[16];
}NET_EHOME_ALARM_INFO, *LPNET_EHOME_ALARM_INFO;

typedef struct tagNET_EHOME_GPS_INFO
{
    DWORD    dwSize;
    char    bySampleTime[MAX_TIME_LEN];      //GPS采样时间，设备本地时间，格式：YYYY-MM-DD HH:MM:SS
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //设备注册ID
    char    byDivision[2];                   //division[0]:'E'or'W'(东经/西经), division[1]:'N'or'S'(北纬/南纬)
    BYTE    bySatelites;                     //卫星数量
    BYTE    byPrecision;                     //精度因子，原始值*100
    DWORD    dwLongitude;                     //经度，取值范围为（0～180*3600*100），转换公式为：longitude= 实际度*3600*100+实际分*60*100+实际秒*100
    DWORD    dwLatitude;                      //纬度，取值范围为（0～90*3600*100），转换公式为：latitude = 实际度*3600*100+实际分*60*100+实际秒*100
    DWORD    dwDirection;                     //方向，取值范围为（0～359.9*100），正北方向为0，转换公式为：direction= 实际方向*100
    DWORD    dwSpeed;                         //速度，取值范围为（0～999.9*100000），转换公式为：speed =实际速度*100000，相当于cm/h
    DWORD    dwHeight;                         //高度，单位：cm
    BYTE    byRetransFlag;                     //重传标记，0-实时包，1-重传包
    BYTE    byLocateMode;                      //定位模式(初值0)，仅NMEA0183 3.00版本输出，值：0-自主定位，1- 差分，2- 估算，3- 数据无效 
    BYTE    byTimeDiffH;  //bySampleTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE    byTimeDiffM;  //bySampleTimee与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    DWORD   dwMileage;                       //设备里程数统计，单位为米，整型，每天第一次开机或者00:00:00时清零，后续累加当天里程数上报到中心 
    BYTE    byRes[56];
}NET_EHOME_GPS_INFO, *LPNET_EHOME_GPS_INFO;

typedef struct tagNET_EHOME_ALARMWIRELESSINFO
{
    BYTE     byDeviceID[MAX_DEVICE_ID_LEN];   //设备ID
    DWORD       dwDataTraffic;    /* 已用流量,单位：MB,实际流量*100*/
    BYTE      bySignalIntensity;  /* 信号强度，0~100*/
    BYTE      byRes[127];
} NET_EHOME_ALARMWIRELESSINFO, *LPNET_EHOME_ALARMWIRELESSINFO;


typedef struct tagNET_EHOME_HEATMAP_VALUE
{
    DWORD dwMaxHeatMapValue;    //最大热度值
    DWORD dwMinHeatMapValue;    //最小热度值
    DWORD dwTimeHeatMapValue;    //平均热度值
}NET_EHOME_HEATMAP_VALUE, *LPNET_EHOME_HEATMAP_VALUE;

typedef struct tagNET_EHOME_PIXEL_ARRAY_SIZE
{
    DWORD dwLineValue;    //像素点行值
    DWORD dwColumnValue;//像素点列值
}NET_EHOME_PIXEL_ARRAY_SIZE, *LPNET_EHOME_PIXEL_ARRAY_SIZE;

typedef struct tagNET_EHOME_HEATMAP_REPORT
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //设备注册ID
    DWORD   dwVideoChannel;                     //通道号
    char    byStartTime[MAX_TIME_LEN];       //开始时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    char    byStopTime[MAX_TIME_LEN];        //结束时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    NET_EHOME_HEATMAP_VALUE struHeatmapValue;//热度值
    NET_EHOME_PIXEL_ARRAY_SIZE struPixelArraySize;    //热度图大小
    char    byPixelArrayData[MAX_URL_LEN];     //热度图数据索引
    BYTE    byRetransFlag;                     //重传标记，0-实时包，1-重传包
    BYTE    byTimeDiffH; //byStartTime，byStopTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE    byTimeDiffM;  //byStartTime，byStopTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE    byRes[61];
}NET_EHOME_HEATMAP_REPORT, *LPNET_EHOME_HEATMAP_REPORT;

typedef struct tagNET_EHOME_HUMAN_FEATURE
{
    BYTE byAgeGroup;    //年龄属性，1-婴幼儿，2-儿童，3-少年，4-青少年，5-青年，6-壮年，7-中年，8-中老年，9-老年
    BYTE bySex;            //性别属性，1-男，2-女
    BYTE byEyeGlass;    //是否戴眼睛，1-不戴，2-戴
    BYTE byRes[13];
}NET_EHOME_HUMAN_FEATURE, *LPNET_EHOME_HUMAN_FEATURE;

typedef struct tagNET_EHOME_FACESNAP_REPORT
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //设备注册ID
    DWORD   dwVideoChannel;                     //通道号
    char    byAlarmTime[MAX_TIME_LEN];       //报警时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    DWORD   dwFacePicID;                     //人脸图ID
    DWORD   dwFaceScore;                     //人脸评分，0-100
    DWORD   dwTargetID;                         //目标ID
    NET_EHOME_ZONE    struTarketZone;             //目标区域，归一化坐标：数值为当前画面的百分比大小*1000，精度为小数点后三位
    NET_EHOME_ZONE  struFacePicZone;         //人脸子图区域，归一化坐标：数值为当前画面的百分比大小*1000，精度为小数点后三位
    NET_EHOME_HUMAN_FEATURE struHumanFeature;//人属性
    DWORD    dwStayDuration;                     //停留画面中时间
    DWORD    dwFacePicLen;                     //人脸图长度，单位：字节
    char    byFacePicUrl[MAX_URL_LEN];         //人脸子图数据索引
    DWORD    dwBackgroudPicLen;                     //背景图片长度，单位：字节
    char    byBackgroudPicUrl[MAX_URL_LEN];         //背景图片数据索引
    BYTE    byRetransFlag;                     //重传标记，0-实时包，1-重传包
    BYTE    byTimeDiffH;  //byAlamTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
    BYTE    byTimeDiffM;  //byAlamTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE    byRes[61];
}NET_EHOME_FACESNAP_REPORT, *LPNET_EHOME_FACESNAP_REPORT;

typedef struct tagNET_EHOME_CID_PARAM
{
    DWORD dwUserType;    //用户类型，1键盘用户 2网络用户，其他值表示无效
    LONG  lUserNo;        //用户类型，-1表示无效
    LONG  lZoneNo;        //防区号，-1表示无效
    LONG  lKeyboardNo;    //键盘号，
    LONG  lVideoChanNo;    //视频通道号
    LONG  lDiskNo;        //硬盘号
    LONG  lModuleAddr;    //模块地址
    char  byUserName[NAME_LEN];    //用户名
    BYTE  byRes[32];
}NET_EHOME_CID_PARAM, *LPNET_EHOME_CID_PARAM;

typedef struct tagNET_EHOME_CID_INFO
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];		//设备注册ID
    DWORD   dwCIDCode;							//CID报告代码
    DWORD   dwCIDType;							//CID报警类型
    DWORD   dwSubSysNo;                         //产生报告的子系统号，0为全局报告，子系统范围0~32
    char    byCIDDescribe[CID_DES_LEN];         //CID报警描述
    char    byTriggerTime[MAX_TIME_LEN];		//CID报警发生时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    char    byUploadTime[MAX_TIME_LEN];         //CID报告上传时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    NET_EHOME_CID_PARAM struCIDParam;			//CID报警参数
	BYTE 	byTimeDiffH;						//byTriggerTime，byUploadTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
	BYTE 	byTimeDiffM;						//byTriggerTime，byUploadTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE    byRes[62];
}NET_EHOME_CID_INFO, *LPNET_EHOME_CID_INFO;

typedef struct tagNET_EHOME_NOTICE_PICURL
{
    DWORD   dwSize;
    char    byDeviceID[MAX_DEVICE_ID_LEN];   //设备注册ID
    WORD    wPicType;    //图片业务类型，0-PU定时抓图上传中心，1-PU报警抓图上传中心(AlarmType、AlarmChan、AlarmTime 三个节点在PicType为时才有效) ，2-CU手动抓图上传中心，
    //3-CU下载图片，4-外部触发PU抓图上传中心（比如DVR通过串口接收到POS机刷卡触发抓图请求）
    WORD    wAlarmType;					//报警类型，6-报警输入，7-视频遮挡，8-移动侦测，11-PIR报警，12-门磁报警，13-呼救报警    
    DWORD   dwAlarmChan;				//报警通道编号
    char    byAlarmTime[MAX_TIME_LEN];  //报警产生时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    DWORD   dwCaptureChan;				//图片抓拍通道编号
    char    byPicTime[MAX_TIME_LEN];    //图片时间（设备本地时间），格式：YYYY-MM-DD HH:MM:SS
    char    byPicUrl[MAX_URL_LEN];      //图片数据索引
    DWORD   dwManualSnapSeq;			//请求流水号（字节整数），PicType值为2、3时有效
    BYTE    byRetransFlag;              //重传标记，0-实时包，-重传包
	BYTE 	byTimeDiffH;				//byAlarmTime，byPicTime与国际标准时间（UTC）的时差（小时），-12 ... +14,0xff表示无效
	BYTE 	byTimeDiffM;				//byAlarmTime，byPicTime与国际标准时间（UTC）的时差（分钟），-30,0, 30, 45, 0xff表示无效
    BYTE    byRes[29];
}NET_EHOME_NOTICE_PICURL, *LPNET_EHOME_NOTICE_PICURL;

typedef struct tagNET_EHOME_NOTIFY_FAIL_INFO
{
    DWORD dwSize;
    char  byDeviceID[MAX_DEVICE_ID_LEN];	//设备注册ID
    WORD  wFailedCommand;					//失败的命令，1-抓图失败
    WORD  wPicType;							//图片业务类型，失败命令为抓图失败时有效，0-PU定时抓图上传中心，1-PU报警抓图上传中心，2-CU手动抓图上传中心，
    //3-CU下载图片，4-外部触发PU抓图上传中心    
    DWORD    dwManualSnapSeq;				//请求流水号（字节整数），PicType值为2、3时有效
    BYTE    byRetransFlag;                  //重传标记，0-实时包，1-重传包    
    BYTE    byRes[31];
}NET_EHOME_NOTIFY_FAIL_INFO, *LPNET_EHOME_NOTIFY_FAIL_INFO;

typedef struct tagNET_EHOME_ALARM_MSG
{
    DWORD dwAlarmType;      //报警类型，见EN_ALARM_TYPE
    void *pAlarmInfo;       //报警内容（结构体）
    DWORD dwAlarmInfoLen;   //结构体报警内容长度
    void *pXmlBuf;          //报警内容（XML）
    DWORD dwXmlBufLen;      //xml报警内容长度
    char  sSerialNumber[NET_EHOME_SERIAL_LEN]; //设备序列号，用于进行Token认证
    BYTE  byRes[20];
}NET_EHOME_ALARM_MSG, *LPNET_EHOME_ALARM_MSG;

typedef BOOL (CALLBACK *EHomeMsgCallBack)(LONG iHandle, NET_EHOME_ALARM_MSG *pAlarmMsg, void *pUser);

typedef struct tagNET_EHOME_ALARM_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struAddress;  //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，多个网卡的情况下，默认为从操作系统获取到的第一个
    EHomeMsgCallBack    fnMsgCb; //报警信息回调函数
    void * pUserData;   //用户数据
    BYTE  byProtocolType;  //协议类型，0-TCP,1-UDP
    BYTE  byUseCmsPort; //是否复用CMS端口,0-不复用，非0-复用，如果复用cms端口，协议类型字段无效（此时本地监听信息struAddress填本地回环地址）
    BYTE  byUseThreadPool;  //0-回调报警时，使用线程池，1-回调报警时，不使用线程池，默认情况下，报警回调的时候，使用线程池
    BYTE  byRes[29];
}NET_EHOME_ALARM_LISTEN_PARAM, *LPNET_EHOME_ALARM_LISTEN_PARAM;


//初始化，反初始化
NET_DVR_API BOOL  CALLBACK NET_EALARM_Init();
NET_DVR_API BOOL  CALLBACK NET_EALARM_Fini();

//获取错误码
NET_DVR_API DWORD CALLBACK NET_EALARM_GetLastError();

//日志
NET_DVR_API BOOL CALLBACK NET_EALARM_SetLogToFile(LONG iLogLevel, const char *strLogDir, BOOL bAutoDel);

//获取版本号
NET_DVR_API DWORD CALLBACK NET_EALARM_GetBuildVersion();

NET_DVR_API LONG CALLBACK NET_EALARM_StartListen(NET_EHOME_ALARM_LISTEN_PARAM *pAlarmListenParam);

NET_DVR_API BOOL CALLBACK NET_EALARM_StopListen(LONG iListenHandle);


NET_DVR_API BOOL  CALLBACK NET_EALARM_SetSDKLocalCfg(  NET_EHOME_LOCAL_CFG_TYPE enumType, void* const lpInBuff);
NET_DVR_API BOOL  CALLBACK NET_EALARM_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType,void *lpOutBuff);



#endif //_HC_EHOME_ALARM_H_