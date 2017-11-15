#include <pthread.h>
#include <unistd.h>
#include "util/util.h"
#include "transcode/cts_main.h"
#include "dispath/taskdispath.h"
#include "util/licence.h"
#include "util/simple_log.h"
#include "http/http_client.h"
#include "util/errorcode.h"
#include "util/oss_ftp_file.h"
extern string g_backup;
extern TaskDispath *g_dispath;
//extern CTS_DB *g_db;
extern string g_path;
extern string g_oss_path;
extern string g_tomcat_path;
extern OSS_FTP_FILE * g_oss_ftp;

CTS_Main::CTS_Main()
{
	//LoadService();
	this->opt = g_dispath->init(TRANSCODE);
}
CTS_Main::~CTS_Main()
{
}
/*
void CTS_Main::LoadService()
{
	list<DispathNode> seedList;
	list<DispathNode>::iterator seed_iter;
	
	list < SeedType> seed;
	list<SeedType>::iterator sed_iter;
	
	list < CTS_DB_Info > dealList;
	list < CTS_DB_Info > taskList;
	list < CTS_DB_Info > ackList;
	list<CTS_DB_Info>::iterator task_iter;
	
	CTS_DB::getSeedTypeData(seed);
	CTS_DB::getSeedServiceData(seedList);
	CTS_DB::getDealInfoData(dealList);
	for(sed_iter = seed.begin();sed_iter!=seed.end();sed_iter++)
	{
		g_dispath->addSeed((*sed_iter));
	}
	for(seed_iter=seedList.begin();seed_iter!=seedList.end();seed_iter++)
	{
		g_dispath->addDispath((*seed_iter));
	}
	for(task_iter=dealList.begin();task_iter!=dealList.end();task_iter++)
	{
		CTS_Info *taskinfo = new CTS_Info(task_iter->task_id.c_str(),task_iter->task_result.c_str(),task_iter->taskinfo.c_str());
		taskinfo->recordSeedInfo(task_iter->seedinfo);
		DEBUG("deal status <%d>",task_iter->dealstat);
		taskinfo->setDealStatus(task_iter->dealstat);
		this->_taskqueue.addDealTask(taskinfo);
	}
	CTS_DB::getTransInfoData(taskList);
	for(task_iter=taskList.begin();task_iter!=taskList.end();task_iter++)
	{
		CTS_Info *taskinfo = new CTS_Info(task_iter->task_id.c_str(),task_iter->task_result.c_str(),task_iter->taskinfo.c_str());
		taskinfo->setPriority(task_iter->priority);
		this->_taskqueue.addTask(taskinfo,this->opt,true);
	}
	CTS_DB::getAckInfoData(ackList);
	for(task_iter=ackList.begin();task_iter != ackList.end();task_iter++)
	{
		CTS_Info *taskinfo = new CTS_Info(task_iter->task_id.c_str(),task_iter->task_result.c_str(),NULL,task_iter->ecode);
		this->_taskqueue.addAckTask(taskinfo);
	}
}

*/
/*************************************************************/
int CTS_Main::Register(string seedinfo,int use)
{
	INFO("Register %s",seedinfo.c_str());
	return g_dispath->addDispath(seedinfo,use,0,this->opt);
}

string CTS_Main::getVideoInfo(string srcframe,string typepath,const int num,const int ptime,const int pseg,string pixels,string &ttime)
{
	char str[8];
	string src_path = g_tomcat_path;
	string oss_path = g_oss_path;
	string str_addr = UTIL::getlocaltime();

	oss_path.append(typepath).append("/").append(str_addr);
	UTIL::CreatDir(oss_path.c_str());

	src_path.append("/").append(srcframe);

	for(int i=0;i<num;i++)
	{
		char str_name[128];
		string cmd("ffmpeg ");
		
		sprintf(str_name,"%s%d.jpg",oss_path.c_str(),i+1);
		
		memset(str,0,sizeof(str));
		UTIL::myitoa(ptime+i*pseg,str,10);
		
		cmd.append(" -y -ss ").append(str).append(" -i ").append(src_path).append(" -vframes 1 ").append(" -s ").append(pixels).append(" ").append(str_name);
		DEBUG("%s",cmd.c_str());
		system(cmd.c_str());
		int upload = 5;
		while(upload--)
		{		
				char oss_str_name[128];
				memset(oss_str_name,0,sizeof(oss_str_name));
				sprintf(oss_str_name,"%s/%s%d.jpg",typepath.c_str(),str_addr.c_str(),i);
				if(access(str_name,0) == 0)
				{
					INFO("begin put object from file [%s]!",oss_str_name);
					if(g_oss_ftp->put_object_from_file(str_name,oss_str_name) == 0)
					{
						remove(str_name);
						break;
					}
					sleep(5);
				}
		}
	}
	ttime = this->getVideoTime(src_path.c_str());
	return str_addr;
}

int CTS_Main::ackTask(string task_id,int stat,int result)
{
	DEBUG(" ack task %s",task_id.c_str());
	if(!stat)
		this->_taskqueue.delDealTask(task_id.c_str(),this->opt);
	else
		this->_taskqueue.recodeAckTask(task_id.c_str(),result,this->opt);
	return 0;
}

int CTS_Main::ParseXml(CTS_Info *taskinfo,const char * filename)
{
	file<> fdoc(filename);
	xml_document<> doc;
	doc.parse<0>(fdoc.data());
	return taskinfo->parseAll(&doc);
}
int CTS_Main::DealInfo(char *task)
{
	CTS_Info *taskinfo = new CTS_Info(g_backup.c_str());
	xml_document<> doc;
	if(!task)
	{
		WARN("task info is NULL!");
		return TASK_EMPTY;
	}
	if(doc.parse<0>(task) == 0)
	{
		if(taskinfo->parseAll(&doc) == 0)
			this->_taskqueue.addTask(taskinfo,this->opt);
	}	
	else
	{
		delete taskinfo;
		ERROR("doc.parse error - %s",task);
		return TASK_FORMAT;
	}
	return SUCCESS;
}
/*inno 默认为0，为0时表示取消任务，不为0时表示取消任务中的一个视频infile*/
int CTS_Main::CancelTask(const char *task_id)
{
	INFO("cancel the task! task id %s",task_id);
	return this->_taskqueue.celTask(task_id,this->opt);
}
/*************************************************************/

void CTS_Main::run()
{
	do
	{
		this->_taskqueue.checkDealStats(this->opt);
		this->_taskqueue.delAckTask();
		string seedinfo = g_dispath->getDispathNodeInfo(this->opt);
		if(!seedinfo.empty())
		{
			this->dispatch(seedinfo);
		}
		else
		{
			sleep(5);
		}
	}while(1);
}
void CTS_Main::dispatch(string seedinfo)
{
	CTS_Info *taskinfo=NULL;
	int ret = 0;
	if(this->_taskqueue.getDealTask(taskinfo,this->opt))
	{
		taskinfo->recordSeedInfo(seedinfo);		
		taskinfo->setDealStatus(DEALING);
		CTS_DB::setDealInfoData(taskinfo->getId(),taskinfo->getSeedInfo(),DEALING);			
		g_dispath->acquireDistpathNode(seedinfo,this->opt);
		DEBUG("service %s dispath task %s",seedinfo.c_str(),taskinfo->getId());
		ret = this->postTaskInfo(seedinfo,taskinfo);
		if(!ret)
		{
			return;
		}
		DEBUG("%s get deal task post [%s] failed [errorcode %d]!",taskinfo->getId(),seedinfo.c_str(),ret);
		g_dispath->releaseDistpathNode(seedinfo,this->opt);
	}
	else if(this->_taskqueue.getTask(taskinfo,this->opt))
	{
		taskinfo->recordSeedInfo(seedinfo);
		taskinfo->setDealStatus(DEALING);
		CTS_DB::addDealInfoData(taskinfo->getId(),taskinfo->getFilename(),taskinfo->getResult(),taskinfo->getSeedInfo(),this->opt,DEALING);
		g_dispath->acquireDistpathNode(seedinfo,this->opt);
		DEBUG("service %s dispath task %s",seedinfo.c_str(),taskinfo->getId());
		ret = this->postTaskInfo(seedinfo,taskinfo);
		if(!ret)
		{
			return;
		}
		DEBUG("%s get task post [%s] failed [errorcode %d]!",taskinfo->getId(),seedinfo.c_str(),ret);
		g_dispath->releaseDistpathNode(seedinfo,this->opt);
	}
	sleep(5);
	return;
}

int CTS_Main::postTaskInfo(string seedinfo,CTS_Info *taskinfo)
{
	CHttpClient seed;
	seed.SetDebug(true);
	char ipaddr[128];
	string responseBuf;
	memset(ipaddr,0,sizeof(ipaddr));	
	sprintf(ipaddr,"%s/transcode",seedinfo.c_str());
	if(access(taskinfo->getFilename(),0) != 0)
		return -1;
	file<> fdoc(taskinfo->getFilename()); 
	string str = fdoc.data();
	return seed.Post(ipaddr,str,responseBuf);
}

string CTS_Main::getVideoTime(const char * filename)
{
	int ret;
    string ttime;
    AVFormatContext *ifmt_ctx = NULL;
    avcodec_register_all();
    av_register_all();
    avformat_network_init();
    ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL);
    if (ret < 0)
    {
    		avformat_close_input(&ifmt_ctx);
            return ttime;
    }
    ret = avformat_find_stream_info(ifmt_ctx, NULL);
    if (ret < 0)
    {
            avformat_close_input(&ifmt_ctx);
            return ttime;
    }
    av_dump_format(ifmt_ctx, 0, filename, 0);
    char buffer[16];
    if (ifmt_ctx->duration != AV_NOPTS_VALUE)
    {
            int hours, mins, secs, us;
            memset(buffer,0,sizeof(buffer));
            secs = ifmt_ctx->duration / AV_TIME_BASE;
            us = ifmt_ctx->duration % AV_TIME_BASE;
            mins = secs / 60;
            secs %= 60;
            hours = mins / 60;
            mins %= 60;
            sprintf(buffer,"%02d:%02d:%02d.%02d",hours, mins, secs, (100 * us) / AV_TIME_BASE);
			ttime = buffer;
    }
    else
    {
    }
    avformat_close_input(&ifmt_ctx);
    return ttime;
}