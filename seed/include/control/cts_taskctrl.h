#ifndef CTS_TASKCTRL_H__
#define CTS_TASKCTRL_H__

#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sched.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "control/cts_taskqueue.h"
#include "control/cts_taskinfo.h"
#include "control/cts_transerv.h"

class CTS_TaskCtrl
{
public:
	CTS_TaskCtrl();
	CTS_TaskCtrl(std::string masterServer,std::string seedIP,std::string serverPort,std::string cachefile);
	~CTS_TaskCtrl();	
	void LoadService(std::string masterServer,std::string seedIP,std::string serverPort);
	int DealInfo(char *task);
	int ParseXml(CTS_TaskInfo *taskinfo,const char * filename);
	int CancelTask(const char *task_id);
	void DelTask(const char *task_id);
	void dispatch(CTS_TaskInfo * taskinfo);
	int uncommitted();
public:
	std::string masterServerUrl;
	std::string cache_file;
	CTS_TaskQueue _taskqueue;
private:
	CTS_Transerv transerv;
};

#endif 
