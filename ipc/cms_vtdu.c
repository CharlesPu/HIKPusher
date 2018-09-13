/*******************************
@@Author     :  Charles
@@Date       :  2017-06-25
@@Mail       :  pu17rui@sina.com
@@Description: 
                HIKPusher
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 
#include "config.h"
#include "cms_vtdu.h"
#include "ipcs.h"
#include "ps-rtmp.h"
#include "log.h"

extern struct _ipc IPCs[IPCS_MAX_NUM];
//注册回调函数, CALLBACK is not defined as any value! (HCEHomePublic.h)
BOOL CALLBACK RegisterCallBack(LONG lUserID, DWORD dwDataType, void *pOutBuffer, DWORD dwOutLen, void *pInBuffer, DWORD dwInLen, void *pUser)
{
    NET_EHOME_DEV_REG_INFO *pDevInfo = (NET_EHOME_DEV_REG_INFO *)pOutBuffer;
    if (ENUM_DEV_ON == dwDataType)
    {  
       if (pDevInfo != NULL)
       {
          unsigned short int_dev_id = IPCS_GetInt_Devid(pDevInfo->byDeviceID, strlen((char*)(pDevInfo->byDeviceID)));
          IPCs[int_dev_id].login_id = lUserID;
          IPCs[int_dev_id].online_state = IPCS_ONLINE;
          LOG_Print(ERR_NONE, "On-line, lUserID: %ld, Device ID: %s\n", IPCs[int_dev_id].login_id, pDevInfo->byDeviceID);
       }
       //输入参数
       NET_EHOME_SERVER_INFO *pServerInfo = (NET_EHOME_SERVER_INFO *)pInBuffer;
       pServerInfo->dwTimeOutCount = 6; //心跳超时次数
       pServerInfo->dwKeepAliveSec = 5; //心跳间隔
    }
    else if (ENUM_DEV_OFF == dwDataType)
    {
        LOG_Print(ERR_NONE, "Off-line, lUserID: %d", lUserID);
        for (int i = 0; i < IPCS_MAX_NUM; ++i)
        {
            if (IPCs[i].login_id == lUserID)
            {
                IPCS_PushFree(&(IPCs[i]));
                IPCs[i].online_state        = IPCS_OFFLINE;
                IPCs[i].push_state          = IPCS_NOT_PUSHING_STREAM;
                IPCs[i].login_id            = -1;
                IPCs[i].stream_handle       = -1;
                IPCs[i].preview_session_id  = -1;
                LOG_Print(ERR_NONE, ", Device ID: %s\n", IPCs[i].dev_id);
                break;
            }
        }
    	NET_ECMS_ForceLogout(lUserID);       
    }
    else
    {
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//实时流数据处理
// static FILE *Videofile = NULL;
BOOL InputStreamData(long int_dev_id, BYTE byDataType, char* pBuffer, int iDataLen)
{	
    
    PS_SendToRtmp(&(IPCs[int_dev_id]), pBuffer, iDataLen);
    // if(Videofile == NULL)
    // {
    //     Videofile = fopen("test.rtp","wb");
    //     printf("Save data to file: test.rtp!\n");
    // }

    // if(Videofile!= NULL)
    // {
    //     printf("len:%d buf:\n", iDataLen);
    //     for (int i = 0; i < iDataLen; ++i)
    //         printf("0x%02x ", (unsigned char)pBuffer[i]);
    //     printf("\n");
    //     fwrite(pBuffer,iDataLen,1,Videofile);  //回调实时流直接写文件，保存录像
    // }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//实时流数据回调函数
void CALLBACK fnPREVIEW_DATA_CB(LONG lPreviewHandle, NET_EHOME_PREVIEW_CB_MSG *pPreviewCBMsg, void *pUserData)
{
    if (NULL == pPreviewCBMsg)
    {
        return ;
    }
    long int_dev_id = -1;
    for (int i = 0; i < IPCS_MAX_NUM; ++i)
    {
        if (IPCs[i].stream_handle == lPreviewHandle)
        {
            int_dev_id = i;
            // printf("int_dev_id%d, Device ID: %s\n", i, IPCs[i].dev_id);
            break;
        }
    }
    InputStreamData(int_dev_id, pPreviewCBMsg->byDataType, (char*)pPreviewCBMsg->pRecvdata, pPreviewCBMsg->dwDataLen);
}

//////////////////////////////////////////////////////////////////////
//VTDU预览请求回应回调函数
BOOL CALLBACK fnPREVIEW_NEWLINK_CB(LONG lPreviewHandle, NET_EHOME_NEWLINK_CB_MSG *pNewLinkCBMsg, void *pUserData)
{
    LOG_Print(ERR_NONE, "Callback of preview listening, Device ID: %s, Channel: %d\n", pNewLinkCBMsg->szDeviceID, pNewLinkCBMsg->dwChannelNo);
    unsigned short int_dev_id = IPCS_GetInt_Devid(pNewLinkCBMsg->szDeviceID, strlen((char*)(pNewLinkCBMsg->szDeviceID)));
    IPCs[int_dev_id].stream_handle = lPreviewHandle;//printf("hand%ld\n", IPCs[int_dev_id].stream_handle);
    //预览数据回调参数
    NET_EHOME_PREVIEW_DATA_CB_PARAM struDataCB = {0};
    struDataCB.fnPreviewDataCB = fnPREVIEW_DATA_CB;
    //struDataCB.byStreamFormat = 0;//封装格式：0- PS 

    if (!NET_ESTREAM_SetPreviewDataCB(IPCs[int_dev_id].stream_handle, &struDataCB))
    {
        LOG_Print(ERR_NONE, "NET_ESTREAM_SetPreviewDataCB failed, error code: %d\n", NET_ESTREAM_GetLastError());
        return FALSE;
    }
    LOG_Print(ERR_NONE, "NET_ESTREAM_SetPreviewDataCB!\n");

    return TRUE;
}
