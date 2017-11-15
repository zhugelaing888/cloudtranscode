#ifndef __TASKCTRL_H__
#define __TASKCTRL_H__

#include "transcode/cts_queue.h"
#include "transcode/cts_info.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sched.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRANSCODE "transcode"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
};


class CTS_Main
{
public:
	CTS_Main();
	~CTS_Main();
	
	void LoadService();

	int Register(string seedinfo,int use);
	string getVideoInfo(std::string srcframe,std::string typepath,const int num,const int ptime,const int pseg,std::string pixels,std::string &ttime);
	string getVideoTime(const char * filename);
	int ackTask(string task_id,int stat,int result);
	int DealInfo(char *task);
	int CancelTask(const char *task_id);
	void run();
private:
	int ParseXml(CTS_Info *taskinfo,const char * filename);
	void dispatch(string seedinfo);
	int postTaskInfo(string seedinfo,CTS_Info *taskinfo);
private:
	int opt;
	CTS_Queue _taskqueue;
};

#endif 
