#include "util/cts_util.h"
#include "control/cts_taskctrl.h"
#include <pthread.h>
#include <unistd.h>
#include "util/cts_log.h"
#include "util/cts_util.h"
#include "http/cts_http_client.h"
#include "util/cts_db.h"

extern pthread_mutex_t m_Mutex;
extern CTS_TaskCtrl *ctrl;
extern CTS_DB db;
extern string g_config_path;

void* threadTask(void *info)
{
	CTS_Transerv transerv;
	cts_ack tmp={0,0};
	CTS_TaskInfo *taskinfo = (CTS_TaskInfo *)info;
	INFO("the thread begin deal the task! task id :%s",taskinfo->getId());
	tmp.result= transerv.TransCode(taskinfo);
	tmp.task_id.append(taskinfo->getId());
	if(taskinfo->UpdateAck(ctrl->masterServerUrl,tmp.status,tmp.result) != 0)
	{
		pthread_mutex_lock(&m_Mutex);
		ctrl->_taskqueue.acklist.push_back(tmp);
		//db.addData(taskinfo->getId(),tmp.status,tmp.result);
		pthread_mutex_unlock(&m_Mutex);
	}
	INFO("the thread deal task finished! task id :%s",taskinfo->getId());
	ctrl->DelTask(taskinfo->getId());
	pthread_exit(0);
}

CTS_TaskCtrl::CTS_TaskCtrl()
{
}
CTS_TaskCtrl::CTS_TaskCtrl(std::string masterServer,std::string seedIP,std::string serverPort,std::string cachefile):masterServerUrl(masterServer),cache_file(cachefile)
{
	LoadService(masterServer,seedIP,serverPort);
}
CTS_TaskCtrl::~CTS_TaskCtrl()
{
}
void CTS_TaskCtrl::LoadService(std::string masterServer,std::string seedIP,std::string serverPort)
{
	CTS_CHttpClient client;	
	int MaxThreadNum = sysconf(_SC_NPROCESSORS_CONF);	
	INFO("the program run simultaneously max :%d",MaxThreadNum);
	string responseBuf;
	char url[512];
	char Max[64];
	UTIL::myitoa(MaxThreadNum,Max,10);
	string str("url=http://");
	str.append(seedIP).append(":").append(serverPort).append("&processNum=");
	str.append(Max);
	sprintf(url,"http://%s/registerTrans",masterServer.c_str());
	if(client.Post(url,str,responseBuf) != 0)
	{
		while(1)
		{
			if(client.Post(url,str,responseBuf) == 0)
				break;
			sleep(5);
		}
	}
	//db.getData(_taskqueue.acklist);
	INFO("register master server success [%s]",masterServer.c_str());
}
int CTS_TaskCtrl::ParseXml(CTS_TaskInfo *taskinfo,const char * filename)
{
	file<> fdoc(filename);
	xml_document<> doc;
	doc.parse<0>(fdoc.data());
	return taskinfo->parseAll(&doc);
}
int CTS_TaskCtrl::DealInfo(char *task)
{
	CTS_TaskInfo *taskinfo = new CTS_TaskInfo(this->cache_file);
	xml_document<> doc;
	if(!task)
	{
		WARN("task info is NULL!");
		return -1;
	}
	if(doc.parse<0>(task) == 0)
	{
		if(taskinfo->parseAll(&doc) == 0)
			this->_taskqueue.addTask(taskinfo);
		this->dispatch(taskinfo);
	}	
	else
	{
		delete taskinfo;
		ERROR("doc.parse error - %s",task);
	}
	return 0;
}

/*inno 默认为0，为0时表示取消任务，不为0时表示取消任务中的一个视频infile*/
int CTS_TaskCtrl::CancelTask(const char *task_id)
{
	return this->_taskqueue.celTask(task_id);
}

void CTS_TaskCtrl::DelTask(const char *task_id)
{
	return this->_taskqueue.delTask(task_id);
}

void CTS_TaskCtrl::dispatch(CTS_TaskInfo *taskinfo)
{
	pthread_t m_ThreadID;
	int ret;
	INFO("begin dispath task!  task id :%s",taskinfo->getId());
	pthread_mutex_lock(&m_Mutex);
	ret = pthread_create(&m_ThreadID,NULL,&threadTask,(void *)taskinfo);
	if (0 != ret)
	{
		ERROR("create the thread error !!!");
	}		
	pthread_mutex_unlock(&m_Mutex);
	INFO("end dispath task!  task id :%s",taskinfo->getId());
	return;	
}

int CTS_TaskCtrl::uncommitted()
{
	if(_taskqueue.acklist.empty())
		return 0;
	else
	{	
		std::string httpath("http://");
		httpath.append(masterServerUrl.c_str()).append("/ackTrans");
		list<cts_ack>::iterator begin;
		pthread_mutex_lock(&m_Mutex);
		for(begin=_taskqueue.acklist.begin();begin != _taskqueue.acklist.end();begin++)
		{
			CTS_CHttpClient tmp;
			std::string response;
			std::string cmd("content=");
			Json::Value arrayItem;
			arrayItem["task_id"]=begin->task_id;
			arrayItem["stat"]=begin->status;
			arrayItem["result"]=begin->result;
			cmd.append(arrayItem.toStyledString());
			INFO("update master ack %s%s",httpath.c_str(),cmd.c_str());
			
   			if(tmp.Post(httpath,cmd,response) == 0)
   			{
   				//db.delData(begin->task_id);
   				begin=_taskqueue.acklist.erase(begin);				
   			}
		}
		pthread_mutex_unlock(&m_Mutex);
	}
	return 0;
}

