#ifndef __CMS_H__
#define __CMS_H__

#include "HCEHomeCMS.h"
#include "HCEHomeStream.h"

BOOL CALLBACK RegisterCallBack(LONG lUserID, DWORD dwDataType, void *pOutBuffer, DWORD dwOutLen, void *pInBuffer, DWORD dwInLen, void *pUser);
BOOL InputStreamData(BYTE byDataType, char* pBuffer, int iDataLen);
void CALLBACK fnPREVIEW_DATA_CB(LONG lPreviewHandle, NET_EHOME_PREVIEW_CB_MSG *pPreviewCBMsg, void *pUserData);
BOOL CALLBACK fnPREVIEW_NEWLINK_CB(LONG lPreviewHandle, NET_EHOME_NEWLINK_CB_MSG *pNewLinkCBMsg, void *pUserData);



#endif