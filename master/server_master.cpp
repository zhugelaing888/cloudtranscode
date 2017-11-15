#include <sstream>
#include <cstdlib>
#include "http/http_server.h"
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "transcode/cts_main.h"
#include "dispath/taskdispath.h"
#include "util/licence.h"
#include "util/simple_log.h"
#include "util/oss_ftp_file.h"
using namespace Json;
using namespace std;
#define BUF_LEN 1028


CTS_Main *g_transcode = NULL;
TaskDispath *g_dispath = NULL;
string g_backup;
string g_infile;
string g_path;
string g_oss_path;
string g_tomcat_path;
OSS_FTP_FILE * g_oss_ftp = NULL;


pthread_mutex_t db_mutex;

void transcode(Request &request, Response &response) 
{
	Json::Value root;
	std::string task = *(request.get_body()->get_raw_string());
		if(task.empty())
		{
			WARN("received task info is NULL");
			root["result"] = "failed";	
		}
		else
		{	
			const int num = task.size()+1;
			char tmp[num];
			sprintf(tmp,"%s",task.c_str());
			if(g_transcode->DealInfo(tmp) == 0)
			{
				root["result"] = "success";			
			}
			else
			{
				root["result"] = "failed";	
			}
		}
	response.set_body(root);
}
void cancelTrans(Request &request, Response &response)
{
	Json::Value root;
	std::string task_id = request.get_param("task_id"); 
	if(task_id.empty())
	{
		WARN("received task info is NULL");
		root["result"] = "failed";	
	}
	else
	{
		if(g_transcode->CancelTask(task_id.c_str()) == 0)
			root["result"] = "success";
		else
			root["result"] = "failed";
	}
	response.set_body(root);
}
void getLicense(Request &request, Json::Value &root)
{	
	int  RunDays;
	int  SurplusDays;
	string  ExpireTime;
	LICENCE::getTime(RunDays,SurplusDays,ExpireTime);
	root["RunDays"]=RunDays;
	root["SurplusDays"]=SurplusDays;
	root["ExpireTime"]=ExpireTime;
}

void getVideoInfo(Request &request,Json::Value &root)
{
	Json::Value listNode;
	char str_name[64];
	int num = 2;
	int ptime = 5;
	int pseg = 5;
	string pixels="640x480";
	string ttime;
	std::string frame = request.get_param("srcframe"); 
	std::string typepath = request.get_param("typepath");
	string addr = g_transcode->getVideoInfo(frame,typepath,num,ptime,pseg,pixels,ttime);
	root["addr"]=frame;
	root["time"]=ttime;
	for(int i=1;i<=num;i++)	
	{
		Json::Value tmp;
		string str_path = g_oss_path;
		memset(str_name,0,sizeof(str_name));
		sprintf(str_name,"%s/%s%d.jpg",typepath.c_str(),addr.c_str(),i);	
		DEBUG("getFrame %s",str_name);
		if(access(str_path.append(str_name).c_str(),0) == 0)
		{
			tmp["sort"] = i;
			tmp["img"] = str_name;
		}
		else
		{
			tmp["sort"] = i;
			tmp["img"] = "";
		}
		listNode.append(tmp);
	}
	root["rows"]=listNode;
}

void getVideoTime(Request &request,Json::Value &root)
{
	/*Json::Value listNode;
	std::string frame = request.get_param("srcframe"); 
	string ttime = g_transcode->getVideoTime(frame.c_str());
	root["time"]=ttime;*/
}

void ackTrans(Request &request, Response &response)
{
	Json::Reader reader;  
    Json::Value arg;  
	Json::Value root;
	reader.parse(request.get_param("content"),arg);
	std::string task_id = arg["task_id"].asString(); 
	int stat = arg["stat"].asInt();
	int result = arg["result"].asInt();
	if(task_id.empty())
	{
		WARN("received task info is NULL");
		root["result"] = "failed";	
	}
	else
	{
		if(g_transcode->ackTask(task_id,stat,result) == 0)
			root["result"] = "success";
		else
			root["result"] = "failed";
	}
	response.set_body(root);
}
void registerTrans(Request &request, Response &response)
{
	Json::Value root;
	std::string url = request.get_param("url"); 
	std::string proc = request.get_param("processNum");
	if(url.empty() || proc.empty())
	{
		WARN("received register trans error!");
		root["result"] = "failed";
	}
	else
	{
		if(g_transcode->Register(url,atoi(proc.c_str())))
			root["result"] = "success";
		else
			root["result"] = "failed";
	}
}

void *TransCodeRun(void *arg)
{	
	g_transcode->run();
	return (void *)NULL;
}
void* WatchService(void *info)
{
	TaskDispath *dispath = (TaskDispath *)info;
	dispath->watchSeedService();
	return (void*)NULL;
}

int init_child_fork(const char *arg)
{
	pthread_t http_id;
	pthread_t watch_id;
	string serverPort;
	string logfile;
	Config configSettings(arg);	
	g_infile = configSettings.Read("infile",g_infile);
	g_backup= configSettings.Read("backup", g_backup);
	g_path = configSettings.Read("path",g_path);
	g_oss_path = configSettings.Read("osspath",g_oss_path);
	g_tomcat_path = configSettings.Read("tomcat",g_tomcat_path);
	logfile = configSettings.Read("log", logfile);
	serverPort = configSettings.Read("serverPort",serverPort);
	g_oss_ftp = new OSS_FTP_FILE(arg);
	log_init(logfile.c_str(),INFO_LEVEL);
	pthread_mutex_init(&db_mutex, NULL);
	g_dispath = new TaskDispath();
	if(pthread_create(&watch_id,NULL,&WatchService,(void*)g_dispath)!=0)
	{
		ERROR("create the thread error!");
		return -1;
	}
	g_transcode = new CTS_Main();
	
	if(pthread_create(&http_id,NULL,&TransCodeRun,(void*)NULL) != 0)
	{
		ERROR("%s","create the thread error !!!");
		return -1;
	}
	HttpServer http_server;
	
	http_server.add_mapping("/transcode",transcode,POST_METHOD);
	http_server.add_mapping("/cancelTrans",cancelTrans,POST_METHOD);
	http_server.add_mapping("/getLicense",getLicense);	
	http_server.add_mapping("/ackTrans",ackTrans,POST_METHOD);
	http_server.add_mapping("/registerTrans",registerTrans,POST_METHOD);
	http_server.add_mapping("/getVideoTime",getVideoTime);
	http_server.add_mapping("/getVideoInfo",getVideoInfo);
	int backlog = 100000;
	int max_events = 1000;
	http_server.start(atoi(serverPort.c_str()), backlog, max_events);
	
	pthread_mutex_destroy(&db_mutex);
//	if(g_db)
	{
//		delete g_db;
//		g_db=NULL;
	}
	if(g_dispath)
	{
		delete g_dispath;
		g_dispath=NULL;
	}
	if(g_transcode)
	{
		delete g_transcode;
		g_transcode=NULL;
	}
	return 0;
}

int main(int argc, char **args) 
{
	//init_child_fork(args[1]);

	pid_t pid, childpid;
 	 int status; 
	if(argc != 2)
	{
		PRINT("argument error ");
		return -1;
	}
  if(daemon(1, 0) != 0) 
  {
      PRINT("Call daemon error" );
      return -1;
  }	
  while(1)
  { 
  		pid = fork();
			if(pid < 0) 
      {
          PRINT("Call fork error");
          break;
      }        
      if(pid == 0) 
      {
         	if (init_child_fork(args[1])< 0) 
          {
              exit(-1);
          }
      }        
     if(pid > 0) 
      {
      	if((childpid = wait(&status)) == -1) 
          {
          	PRINT("error Call wait return %d ",status);
              return -1;
          }
          PRINT("error childpid %d return status %d \n ", childpid, status);
          if (WIFSIGNALED(status)) 
          {
              PRINT("error transcode process interrupted by signal %d \n", WTERMSIG(status));
          }
      }
  }
	return 0;
}
