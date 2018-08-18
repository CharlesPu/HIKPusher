# HIKPusher

A push kit of RTMP based on HikVision's EHome SDK(v4.0) that can accept the registration of HIK's IPCs and push streaming to RTMP server.
Once there is a preview request, this kit can collect the streaming and transform the private RTP into RTMP, and then push it to any RTMP server, such as Red5 or nginx-rtmp module. Hence the real time monitoring. 

------------------------------
## VERSIONS

##### date:			2018-07-15
##### version:		v1.0
##### description:
* Finish the decoding of private RTP to H264 streaming.
* Finish the encoding of H264 streaming to RTMP.
* Combine the dec-enc and HIK's EHome SDK.
* Add a server that used to listen and accept the preview requests. Use heart beat packet to collect the requests.
* Add "ipcs.c", use the global array variable "IPCs[]" to storage every ipc's infos: login_id, dev_id, push rtmp handle, etc.

------------------------------
##### date:			2018-07-16
##### version:		v1.0.2
##### description:
* Add the signal handler function of signal "ctrl + c", hence the release of all memory resources such as req_srv buf and ipc's push buf when use "ctrl + c" to exit the main function. As a result, some variables has changed to global ones due to the reference in signal handler function.

------------------------------
##### date:			2018-07-17
##### version:		v1.0.4
##### description:
* Change every ipc's rtmp url, the end of url is dev_id.
* Change the rtmp ip macro to two: "0.0.0.0" and public net ip.
* Fix the bug when free ipc's push resources.
* Change the project name(from "eh_rtmp_demo" to "HIKPusher").

------------------------------
##### date:			2018-07-21
##### version:		v1.2
##### description:
* **Add each ipc's info: metadata, send flag, tick, etc. when send h264, so as to distinguish different ipcs.** And modify each ipc's push_free function. 
* Because of the loop contains between h264-rtmp.h and ipcs.h, move the definition of RTMPMetadata to ipcs.h.

------------------------------
##### date:			2018-08-18
##### version:		v1.4
##### description:
* Change the directions of the project.
* Add the record module(./record) of the project, hence the log files which can record the problems when running.