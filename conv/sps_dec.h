/*******************************
@@Author     : Charles
@@Date       : 2017-06-17
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#ifndef __SPS_DEC_H__
#define __SPS_DEC_H__ 

typedef  unsigned int SPS_UINT;
typedef  unsigned char SPS_BYTE;
typedef  unsigned long SPS_DWORD;

SPS_UINT Ue(SPS_BYTE *pBuff, SPS_UINT nLen, SPS_UINT* nStartBit);
int Se(SPS_BYTE *pBuff, SPS_UINT nLen, SPS_UINT* nStartBit);
SPS_DWORD u(SPS_UINT BitCount, SPS_BYTE* buf, SPS_UINT* nStartBit);
void de_emulation_prevention(SPS_BYTE* buf, unsigned int buf_size);
int H264_DecodeSPS(SPS_BYTE* buf, unsigned int nLen, int* width, int* height, int* fps);

#endif
