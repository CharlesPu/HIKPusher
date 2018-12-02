/*******************************
@@Author     : Charles
@@Date       : 2018-06-04
@@Mail       : pu17rui@sina.com
@@Description: 
	wish that I can transfer RTP to RTMP successfully!!   
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 
#include <signal.h>
#include "config.h"
#include "cms_vtdu.h"
#include "req_srv.h"
#include "ipcs.h"
#include "log.h"

/*global variables*/
LONG gpreview_listen_handle = 0;
LONG glisten_handle = 0;
struct _srv req_srv;
extern struct _ipc IPCs[IPCS_MAX_NUM];

unsigned long GetTimeInterval(struct timeval* start, struct timeval* end);
void FreeAll(int sig_num);
int main()
{
    LOG_Init();
    signal(SIGINT, FreeAll);//ctrl + c signal handler
    IPCS_Init();
	REQ_ServerInit(&req_srv);
    LOG_INFO("REQ_Listen_Server start listen! port:%d\n", REQ_SRV_PORT);
    //流媒体服务器(VTDU)监听取流
    //取流库初始化
    NET_ESTREAM_Init();
    //预览监听参数
    NET_EHOME_LISTEN_PREVIEW_CFG struListen = {0};
    memcpy(struListen.struIPAdress.szIP,NET_ESTREAM_IP, sizeof(NET_ESTREAM_IP));
    struListen.struIPAdress.wPort = NET_ESTREAM_PORT; //流媒体服务器监听端口
    struListen.fnNewLinkCB = fnPREVIEW_NEWLINK_CB; //预览连接请求回调函数
    struListen.pUser = NULL;
    struListen.byLinkMode = LINK_MODE_TCP; //0- TCP方式，1- UDP方式 
    //启动预览监听
    gpreview_listen_handle = NET_ESTREAM_StartListenPreview(&struListen);
    if(gpreview_listen_handle < -1)
    {
        LOG_ERROR(ERR_VTDU_START, "NET_ESTREAM_StartListenPreview failed, error code: %d\n", NET_ESTREAM_GetLastError());
        NET_ESTREAM_Fini();
        return 1;
    }
    LOG_INFO( "NET_ESTREAM_StartListenPreview! port:%d\n", NET_ESTREAM_PORT);
    ////////////////////////////////////////////////////////////////////////
    //CMS注册和预览请求
    //CMS注册模块初始化
    NET_ECMS_Init();
    //注册监听参数
    NET_EHOME_CMS_LISTEN_PARAM struCMSListenPara = {0};
    memcpy(struCMSListenPara.struAddress.szIP, NET_ESTREAM_IP, sizeof(NET_ESTREAM_IP));
    struCMSListenPara.struAddress.wPort = NET_ECMS_PORT;
    struCMSListenPara.fnCB = RegisterCallBack;
    //启动监听，接收设备注册信息
    glisten_handle = NET_ECMS_StartListen(&struCMSListenPara);
    if(glisten_handle < -1)
    {
        LOG_ERROR(ERR_CMS_START, "NET_ECMS_StartListen failed, error code: %d\n", NET_ECMS_GetLastError());
        NET_ECMS_Fini();
        return 2;
    }
    LOG_INFO("NET_ECMS_StartListen! port:%d\n", NET_ECMS_PORT);

    while(1) 
    {
    	/*epoll为server阻塞监听事件, evts为struct epoll_event类型数组, REQ_SRV_CLIE_MAX_NUM为数组容量, -1表永久阻塞*/
		int fd_num = epoll_wait(req_srv.ep_fd, req_srv.evts, REQ_SRV_CLIE_MAX_NUM, IPCS_EPOLL_WAIT_TIME);
		if (fd_num < 0)
		{
            LOG_ERROR(ERR_EPOLL_WAIT, "epoll_wait fail");
			perror("epoll_wait fail\n");
            sleep(1);
			continue;
		}else
        if (fd_num == 0)
        {
            LOG_DEBUG("There is no preview request now!\n");
            // continue;
        }
        struct timeval time_now;
        gettimeofday(&time_now, NULL); 
		for (int i = 0; i < fd_num; i++)
		{
			if( !(req_srv.evts[i].events & EPOLLIN ))          //如果读到的不是有数据过来了
				continue;
			if (req_srv.evts[i].data.fd == req_srv.listen_fd)  //如果读到的是新的连接,就建立连接先
				REQ_ServerAccept(&req_srv);
			else 
			if( (req_srv.evts[i].events & EPOLLIN) )     //如果读到的是有数据过来了！else不能省略!!
			{
				int sock_tmp = req_srv.evts[i].data.fd;
				int bytes_num = REQ_ServerRecv(sock_tmp, &req_srv);
				if (bytes_num <= 0)
				{//先从红黑树上删除此节点
					int res = epoll_ctl(req_srv.ep_fd, EPOLL_CTL_DEL, sock_tmp, NULL);  
					if (res == -1)
                    {
                        LOG_ERROR(ERR_EPOLL_CTL_DEL, "epoll_ctl_delete fail!");
						perror("epoll_ctl_delete fail!\r\n");
                    }
					close(sock_tmp);//一定要释放掉此socket!!!!
#ifdef PRINT_REQ_LINK
					LOG_INFO("%s:%d, clie_fd:%d closed connection\r\n"
                        , inet_ntoa(req_srv.clie_addr.sin_addr), ntohs(req_srv.clie_addr.sin_port), sock_tmp);
#endif	
    			}else 
				{		
                    //reg_pack : I P C S 00 00 0X 0X
                    //receive_pack : s t a r t : I P C S 00 00 0X 0X
                    unsigned short int_dev_id = IPCS_GetInt_Devid(req_srv.rxbuf + 6, bytes_num - 6);
                    memcpy(&(IPCs[int_dev_id].last_req_time), &time_now, sizeof(struct timeval));
                    if (IPCs[int_dev_id].push_state == IPCS_NOT_PUSHING_STREAM// must be not pushing!
                        && IPCs[int_dev_id].online_state == IPCS_ONLINE)//must be on line!
                    { 
                    /***********************start pushing!!!************************/ 
                        // if (IPCs[int_dev_id].online_state == IPCS_OFFLINE
                        //     || IPCs[int_dev_id].push_state == IPCS_PUSHING_STREAM) 
                        //     continue;
                        LOG_INFO("Starting device_id:%s, login_id:%ld .......\n"
                            , IPCs[int_dev_id].dev_id, IPCs[int_dev_id].login_id); 
                        if (IPCS_PushInit(&(IPCs[int_dev_id])))
                        {
                            LOG_ERROR(ERR_IPC_PUSH_INIT, "RTMP_init failed! dev_id:%s, login_id:%ld\n"
                                , IPCs[int_dev_id].dev_id, IPCs[int_dev_id].login_id);
                            IPCS_PushFree(&(IPCs[int_dev_id]));
                            continue;
                        }
                        //预览请求输入参数
                        NET_EHOME_PREVIEWINFO_IN_V11 struPreviewIn = {0};
                        struPreviewIn.iChannel = CHANNEL_ID; //通道号
                        struPreviewIn.dwLinkMode = LINK_MODE_TCP; //0- TCP方式，1- UDP方式 
                        struPreviewIn.dwStreamType = STREAM_TYPE_SUB; //码流类型：0- 主码流，1- 子码流, 2- 第三码流
                        memcpy(struPreviewIn.struStreamSever.szIP, NET_ESTREAM_PUBLIC_IP, sizeof(NET_ESTREAM_PUBLIC_IP));//流媒体服务器IP地址
                        struPreviewIn.struStreamSever.wPort = NET_ESTREAM_PORT; //流媒体服务器端口，需要跟服务器启动监听端口一致
                        //预览请求输出参数
                        NET_EHOME_PREVIEWINFO_OUT struPreviewOut = {0};
                        //预览请求
                        if(!NET_ECMS_StartGetRealStreamV11(IPCs[int_dev_id].login_id, &struPreviewIn, &struPreviewOut))
                        {
                            LOG_ERROR(ERR_CMS_STREAM_START, "NET_ECMS_StartGetRealStreamV11 failed, error code: %d\n", NET_ECMS_GetLastError());
                            IPCS_PushFree(&(IPCs[int_dev_id]));
                            continue;
                        }
                        IPCs[int_dev_id].preview_session_id = struPreviewOut.lSessionID;
                        // LOG_Print(ERR_NONE, "NET_ECMS_StartGetRealStreamV11!\n");
                        //预览请求推流输入参数 
                        NET_EHOME_PUSHSTREAM_IN struPushStreamIn = {0};
                        struPushStreamIn.dwSize = sizeof(struPushStreamIn);
                        struPushStreamIn.lSessionID = IPCs[int_dev_id].preview_session_id; //SessionID，预览请求会话ID
                        //预览请求推流输出参数 
                        NET_EHOME_PUSHSTREAM_OUT struPushStreamOut = {0};

                        //向设备发送命令请求开始发送实时码流，EHOME协议版本大于等于4.0的设备支持
                        if(!NET_ECMS_StartPushRealStream(IPCs[int_dev_id].login_id, &struPushStreamIn, &struPushStreamOut))
                        {
                            LOG_ERROR(ERR_CMS_STREAM_PUSH, "NET_ECMS_StartPushRealStream failed, error code: %d\n", NET_ECMS_GetLastError());
                            IPCS_PushFree(&(IPCs[int_dev_id]));
                            continue;
                        }
                        LOG_INFO("NET_ECMS_StartPushRealStream! dev_id:%s, login_id:%ld\n"
                            , IPCs[int_dev_id].dev_id, IPCs[int_dev_id].login_id);
                        /*remember change the push_state*/
                        IPCs[int_dev_id].push_state = IPCS_PUSHING_STREAM;
                    }
				}
			}		
		}
        /*check every ipc's time interval*/
        unsigned long interval = 0;
        for (int i = 0; i < IPCS_MAX_NUM; ++i)
        {
            if (IPCs[i].push_state == IPCS_PUSHING_STREAM//this ipc has started pushing stream!
                && IPCs[i].online_state == IPCS_ONLINE) //must be on line!
            {
                interval = GetTimeInterval(&(IPCs[i].last_req_time), &time_now); // us
                if ( interval > IPCS_HEARTBEAT_INVL ) //user has left the web page(none heartbeat packs)
                {
                    // if (IPCs[i].online_state == IPCS_OFFLINE
                    //     || IPCs[i].push_state == IPCS_NOT_PUSHING_STREAM)//and must be pushing!
                    //     continue;
                    /**************************stop preview************************/
                    // LOG_Print(ERR_NONE, "[%d]the interval:%ld \n", i, interval);
                    LOG_INFO("Stoping device_id:%s, login_id:%ld .......\n", IPCs[i].dev_id, IPCs[i].login_id);            
                    //释放CMS预览请求资源
                    if(!NET_ECMS_StopGetRealStream(IPCs[i].login_id, IPCs[i].preview_session_id))
                    {
                        LOG_ERROR(ERR_CMS_STREAM_STOP, "NET_ECMS_StopGetRealStream failed, error code: %d\n", NET_ECMS_GetLastError());
                    }
                    //VTDU停止预览
                    if(IPCs[i].stream_handle >= 0)
                    {
                        if (!NET_ESTREAM_StopPreview(IPCs[i].stream_handle))
                        {
                            LOG_ERROR(ERR_VTDU_STOP, "NET_ESTREAM_StopPreview failed, error code: %d\n", NET_ECMS_GetLastError());
                        }
                    }
                    IPCS_PushFree(&(IPCs[i]));
                    LOG_INFO("NET_ECMS_StopPushRealStream! dev_id:%s, login_id:%ld\n", IPCs[i].dev_id, IPCs[i].login_id);
                    /*remember change the push_state*/
                    IPCs[i].push_state = IPCS_NOT_PUSHING_STREAM;
                }
            }
        }
        usleep(1);
    }
    ////////////////////////////////////////////////////////////////////////
    //退出
    FreeAll(0);

    return 0;
}
unsigned long GetTimeInterval(struct timeval* start, struct timeval* end)  
{  
    unsigned long diff = 1000000 * (end->tv_sec - start->tv_sec) + end->tv_usec - start->tv_usec;

    return diff;
} 
void FreeAll(int sig_num)
{
    // LOG_Print(ERR_NONE, "sig_num:%d\n", sig_num);
    //CMS停止监听
    if(!NET_ECMS_StopListen(glisten_handle))
        LOG_ERROR(ERR_CMS_STOP, "NET_ECMS_StopListen failed, error code: %d\n", NET_ECMS_GetLastError());
    //CMS库反初始化，释放资源
    NET_ECMS_Fini();
    //VTDU停止预览监听
    if(gpreview_listen_handle >= 0)
        if (!NET_ESTREAM_StopListenPreview(gpreview_listen_handle))
            LOG_ERROR(ERR_VTDU_STOP, "NET_ESTREAM_StopListenPreview failed, error code: %d\n", NET_ECMS_GetLastError());
    //取流库反初始化，释放资源
    NET_ESTREAM_Fini();
    REQ_FreeServer(&req_srv);  
    for (int i = 0; i < IPCS_MAX_NUM; ++i)
        if (IPCs[i].push_state == IPCS_PUSHING_STREAM)
            IPCS_PushFree(&(IPCs[i]));
    LOG_INFO("Exit!\n");
    LOG_Free();
    exit(0);
}