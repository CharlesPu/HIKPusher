/*******************************
@@Author     : Charles
@@Date       : 2017-06-05
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __RTP_RTMP_H__
#define __RTP_RTMP_H__

#include "ipcs.h"

int RTP_SendToRtmp(struct _ipc *, char* buffer, int length);

#endif