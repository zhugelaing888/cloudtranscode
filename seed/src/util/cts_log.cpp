#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <signal.h> 
#include <stdarg.h>
#include <sys/time.h> 
#include <unistd.h>
#include <string.h>
#include "util/cts_log.h"

static LOG_FILE g_logfile;

static int getLogFilename(char * filename);
static void _log(const char *format, va_list ap);
static int CreatDir(char *pDir);
int getNextTime();
static char * ObtainPath(char *filename,char *filedir);


std::string get_show_time() {
	char show_time[100];
	memset(show_time, 0, 100);
	
	time_t timep;
	struct tm * tm;
	time(&timep);
	tm = localtime(&timep);
	sprintf(show_time, "[%4d%02d%02d %02d%02d%02d]", (1900 + tm->tm_year), (1
			+ tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	return std::string(show_time);
}

int get_log_level(const char *level_str) 
{
	if(strcasecmp(level_str, "ERROR") == 0) 
	{
		return ERROR_LEVEL;
	}
	if(strcasecmp(level_str, "WARN") == 0) 
	{
		return WARN_LEVEL;
	}
	if(strcasecmp(level_str, "INFO") == 0) 
	{
		return INFO_LEVEL;
	}
	if(strcasecmp(level_str, "DEBUG") == 0) 
	{
		return DEBUG_LEVEL;
	}
	return DEBUG_LEVEL;
}

static void UpdateLog(int signo)
{
	char command[512];
	char filename[128];
	getLogFilename(filename);
	pthread_mutex_lock(&g_logfile.m_mutex);
	memset(command, 0x00, sizeof(command));
	sprintf(command, "mv  %s %s", g_logfile.filename,filename);
	system(command);
	pthread_mutex_unlock(&g_logfile.m_mutex);
}

int log_close()
{
	pthread_mutex_destroy(&g_logfile.m_mutex);
	return 0;
}

int log_init(const char *filename,int level)
{
	char file[128];
	pthread_mutex_init(&g_logfile.m_mutex, NULL);
	memcpy(file,filename,128);
	memcpy(g_logfile.filename,file,128);
	CreatDir(file);
	ObtainPath(file,g_logfile.filedir);
	g_logfile.log_level = level;	
	
	struct itimerval tick;    
    signal(SIGALRM, UpdateLog);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 3600*24;
    tick.it_value.tv_usec = 0;
    tick.it_interval.tv_sec = 3600*24;
    tick.it_interval.tv_usec = 0;
    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
           ERROR("Set timer failed!");	
	
	return 0;
}
void log_error(const char *format, ...) 
{
	if (g_logfile.log_level < ERROR_LEVEL) {
		return;
	}
	int len = 0;
	va_list ap;
	va_list aa;
	va_start(aa,format);
	va_start(ap, format);
	len = vsnprintf(NULL,0,format,aa) + 1;
	if(len > 0 && len < 4096)
	{
		_log(format, ap);
	}
	va_end(aa);
	va_end(ap);
}

void log_warn(const char *format, ...) 
{
	if (g_logfile.log_level < WARN_LEVEL) {
		return;
	}
	int len = 0;
	va_list ap;
	va_list aa;
	va_start(aa,format);
	va_start(ap, format);
	len = vsnprintf(NULL,0,format,aa) + 1;
	if(len > 0 && len < 4096)
	{
		_log(format, ap);
	}
	va_end(aa);
	va_end(ap);
}

void log_info(const char *format, ...) 
{
	if (g_logfile.log_level < INFO_LEVEL) {
		return;
	}
	int len = 0;
	va_list ap;
	va_list aa;
	va_start(aa,format);
	va_start(ap, format);
	len = vsnprintf(NULL,0,format,aa) + 1;
	if(len > 0 && len < 4096)
	{
		_log(format, ap);
	}
	va_end(aa);
	va_end(ap);
}

void log_debug(const char *format, ...) 
{
	if (g_logfile.log_level < DEBUG_LEVEL) {
		return;
	}
	int len = 0;
	va_list ap;
	va_list aa;
	va_start(aa,format);
	va_start(ap, format);
	len = vsnprintf(NULL,0,format,aa) + 1;
	if(len > 0 && len < 4096)
	{
		_log(format, ap);
	}
	va_end(aa);
	va_end(ap);
}

void log_print(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	vprintf(format, ap);
	printf("\n");

	va_end(ap);
}
static void _log(const char *format, va_list ap) 
{
	pthread_mutex_lock(&g_logfile.m_mutex);
	std::fstream fs(g_logfile.filename, std::fstream::out | std::fstream::app);
	if(fs.is_open()) 
	{
		char single_log[4096];
		vsprintf(single_log, format, ap);
		fs << single_log << "\n";
		fs.close();
	}
	pthread_mutex_unlock(&g_logfile.m_mutex);
}

/*-----------------------------------------------------------------------
 * name:    创建日志文件名后续时间字符串
 * input:
 * output:  filename  -- 志文件名后续时间字符串
 * return:
 *-----------------------------------------------------------------------*/
static int getLogFilename(char * filename) 
{
	time_t timep;
	struct tm * p;
	char str[20];

	time(&timep);
	p = localtime(&timep);
	
	sprintf(str, "%4d%02d%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	sprintf(filename, "%s/%s.log", g_logfile.filedir, str);
	return 0;
}


int getNextTime()
{
	time_t timep;
	struct tm * tm;
	time(&timep);
	tm = localtime(&timep);
	
	int h_tm = (23 - tm->tm_hour)*60*60;
	int m_tm = (59 - tm->tm_min)*60;
	int s_tm = (60 - tm->tm_sec);
	return h_tm+m_tm+s_tm;
	
}
static char * ObtainPath(char *filename,char *filedir)
{
	int j = 0;
	int len = strlen(filename)+1;
	for (int i = 0;i < len;i++)  
    {  
        if (filename[i] == '\\' || filename[i] == '/')
					j = i; 
				else if(filename[i] == '\0')
				{
					filename[j] = '\0';
					strcpy(filedir,filename);
				}
    }
	return &filename[j+1];
}



static int CreatDir(char *pDir)  
{  
    int i = 0;  
    int iRet;  
    int iLen;  
    char* pszDir;  
  	
    if(NULL == pDir)
    	return 0;
    pszDir = strdup(pDir);  
    iLen = strlen(pszDir); 
		if(pszDir[0] == '/')
			i = 1;
		else
			i=0;
    for (;i < iLen;i++)  
    {  
        if (pszDir[i] == '\\' || pszDir[i] == '/')  
        {   
            pszDir[i] = '\0';   
            iRet = access(pszDir,0);  
            if (iRet != 0)  
            {  
                iRet = MKDIR(pszDir);  
                if (iRet != 0)  
                {  
                    return -1;  
                }
            } 
            pszDir[i] = '/';  
        }
    }  
    free(pszDir);
    return iRet;  
} 

