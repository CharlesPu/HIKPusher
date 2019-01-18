# HIKPusher

A push kit of RTMP based on HikVision's EHome SDK(v4.0) that can accept the registration of HIK's IPCs and push streaming to RTMP server.
Once there is a preview request, this kit can collect the streaming and transform the private RTP into RTMP, and then push it to any RTMP server, such as Red5 or nginx-rtmp module. Hence the real time monitoring. 

----
## software framework
                                      ____________________________________
        requests                     |                                    |========>>>>=======+
        from web        ============>|  REQ_SRV    ============>    CMS   | preview request   |
                           socket    |                                    |                   |
                                     |------------------------------------|                  IPCs
    streaming server                 |                                    |                   |
    (nginx-rtmp, ...)   <============|  librtmp  <== conversion <== VTDU  |========<<<<=======+
                            RTMP     |------------------------------------|    RTP stream
                           stream    |              record, ...           |
                                     |____________________________________|
## deploy tips
It very strange that the link tool--ldd still can't find the **libhpr.so** dynamic library even if I use **-L(hik_ehomelib_path)** and **-l(hik_ehomelib_name)** in makefile.
So you need to add **hik_ehomelib_path** into **/etc/profile** to make the executable program can be created successfully.
> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:(your_path)/HIKPusher/HIK_EHome/eh_lib/

## REQ_SRV protocol 
The request from web in the form of socket is now following the format:
> "start:IPCS00000x0x"

so you need to set the EHome account of HikVision IPC with the format of **"IPCS00000x0x"**, and the first 'x' refers to location, the second one refers to number in this location in order to distinguish every IPC.

The settings above is mine, and you can also set your own protocol, which must change the function **IPCS_GetInt_Devid()** in **ipc/ipcs.c** so as to parse out correct IPC dev_id(EHome account). 

---
## VERSIONS

##### date:       2018-07-15
##### version:    v1.0
##### description:
* Finish the decoding of private RTP to H264 streaming.
* Finish the encoding of H264 streaming to RTMP.
* Combine the dec-enc and HIK's EHome SDK.
* Add a server that used to listen and accept the preview requests. Use heart beat packet to collect the requests.
* Add "ipcs.c", use the global array variable "IPCs[]" to storage every ipc's infos: login_id, dev_id, push rtmp handle, etc.

---
##### date:     2018-07-16
##### version:    v1.0.2
##### description:
* Add the signal handler function of signal "ctrl + c", hence the release of all memory resources such as req_srv buf and ipc's push buf when use "ctrl + c" to exit the main function. As a result, some variables has changed to global ones due to the reference in signal handler function.

---
##### date:       2018-07-17
##### version:    v1.0.4
##### description:
* Change every ipc's rtmp url, the end of url is dev_id.
* Change the rtmp ip macro to two: "0.0.0.0" and public net ip.
* Fix the bug when free ipc's push resources.
* Change the project name(from "eh_rtmp_demo" to "HIKPusher").

---
##### date:       2018-07-21
##### version:    v1.2
##### description:
* **Add each ipc's info: metadata, send flag, tick, etc. when send h264, so as to distinguish different ipcs.** And modify each ipc's push_free function. 
* Because of the loop contains between h264-rtmp.h and ipcs.h, move the definition of RTMPMetadata to ipcs.h.

---
##### date:       2018-08-18
##### version:    v1.4
##### description:
* Change the directions of the project.
* Add the record module(./record) of the project, hence the log files which can record the problems when running.

---
##### date:       2018-09-13
##### version:    v1.4.2
##### description:
* Change the macro name of ipc's states.
* Fix some bugs about the judgements of ipc's states.

---
##### date:       2018-09-14
##### version:    v1.4.4
##### description:
* Change the file name of ps/h264 parser and some function name in 'conv' to look more clear.
* Change the **struct _ipc**, remove 'm_pFileBuf' because its duplication with 'full_h264pack'.

---
##### date:       2018-09-16
##### version:    v1.4.6
##### description:
* Change some functions in parser_h264.c, especially **H264_SendToRtmp**, to make it more simple.
* Change the return value of functions in parser_ps.c.
* Simplify many functions and data structures in the whole project.

---
##### date:       2018-09-20
##### version:    v1.4.8
##### description:
* Change log.c, divide logs into different types--INFO, WARN, ERROR, DEBUG.
* Change the format of record: [INFO/WARN/ERROR/DEBUG time]\([error_num]\) \<line_num\>\<func_name\> content
* Fix some mutex bugs when save log files.

---
##### date:       2019-01-18
##### version:    v2.0.0
##### description:
* Due to the truncation of PS packs during transmission, use two parse buffer: **pespack_buf** and **h264pack_buf** in struct \_ipc to storage a complete pes pack or h264 pack for the following parse.
* Clarify the parse process into two parser: **PS_Parser** and **H264_Parser**, and make them a chain.
* May be used in different kinds of HIK devices...