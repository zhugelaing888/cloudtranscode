#include <sstream>
#include <cstdlib>
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
#include "control/cts_taskctrl.h"
#include "util/cts_log.h"
#include "http/cts_http_server.h"
#include "control/oss_ftp_file.h"


using namespace Json;
using namespace std;
#define BUF_LEN 1028

CTS_TaskCtrl *ctrl = NULL;

pthread_mutex_t m_Mutex;
string g_config_path;
string g_tomcat_path;

OSS_FTP_FILE * g_oss_ftp = NULL;





void status(Request &request, Response &response)
{
	Json::Value root;
	root["status"] = 1;
	ctrl->uncommitted();
	response.set_body(root);
}

void transcode(Request &request, Response &response) 
{
	Json::Value root;
	std::string task = *(request.get_body()->get_raw_string());
	if(task.empty())
	{
		ERROR("received task info is NULL");
		root["result"] = "failed";	
	}
	else
	{	
		const int num = task.size()+1;
		char tmp[num];
		sprintf(tmp,"%s",task.c_str());
		if(ctrl->DealInfo(tmp) == 0)
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
void CancelTask(Request &request, Response &response)
{
	Json::Value root;
	std::string task_id = request.get_param("task_id"); 
	if(ctrl->CancelTask(task_id.c_str()) == 0)
		root["result"] = "success";
	else
		root["result"] = "failed";
	response.set_body(root);
}

int init_child_fork(const char *arg)
{		
	std::string logfile,serverPort,masterServer,seedIP,cachefile;
	Config configSettings(arg);	
	logfile = configSettings.Read("log", logfile);
	serverPort = configSettings.Read("serverPort",serverPort);
	masterServer = configSettings.Read("masterServer",masterServer);
	seedIP = configSettings.Read("seedIP",seedIP);
	cachefile = configSettings.Read("cachefile",cachefile);
	g_tomcat_path = configSettings.Read("tomcat",g_tomcat_path);
	log_init(logfile.c_str(),INFO_LEVEL);
	pthread_mutex_init(&m_Mutex, NULL);	
	g_oss_ftp = new OSS_FTP_FILE(arg);
	ctrl = new CTS_TaskCtrl(masterServer,seedIP,serverPort,cachefile);	
	HttpServer http_server;
	http_server.add_mapping("/transcode",transcode,POST_METHOD);
	http_server.add_mapping("/CancelTask",CancelTask,POST_METHOD);
	http_server.add_mapping("/status",status,POST_METHOD);
	int backlog = 100000;
	int max_events = 5000;
	//http_server.add_bind_ip(seedIP);
	http_server.start(atoi(serverPort.c_str()), backlog, max_events);
	pthread_mutex_destroy(&m_Mutex);
	return 0;
}

int main(int argc, char **argv) 
{
	//init_child_fork(argv[1]);

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
           	if (init_child_fork(argv[1])< 0) 
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
