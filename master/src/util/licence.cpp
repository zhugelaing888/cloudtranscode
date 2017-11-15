#include "util/licence.h"
#include "json/json.h"
#include "util/simple_log.h"
#include "util/util.h"
#include <string.h>

using namespace Json;

extern string g_infile;
static int s_Timer = 0;


static bool TimeError;
static pthread_mutex_t file_Mutex;

static int encrypt(string inBuf)
{
    ofstream fout(g_infile.c_str());
    if( !fout )
    {
            return -1;
    }	
		for(unsigned int i = 0;i < inBuf.size();i++)
		{
			inBuf.at(i) = ~inBuf.at(i)+66;
		}
		fout.write( inBuf.c_str(), inBuf.size() );
    fout.close();
    return 0;
}

static int decrypt(string &outBuf )
{
    fstream fio(g_infile.c_str(), ios::in);
    if( !fio )
    {
            return -1;
    }
    char chArry[SIZE];
    int readNum = 0;
		while( !fio.eof() )
    {
				memset(chArry,0,sizeof(chArry));
        fio.read( chArry, sizeof(chArry) );
        readNum = fio.gcount();
        for( int i = 0; i < readNum; i++ )
        {
            chArry[i] = chArry[i] - 66;
            chArry[i] = ~chArry[i];
        }				
				outBuf.append(chArry);
    }
    fio.close();
    return 0;
}

static void addDays(int signo)
{
	if(s_Timer == 0)
	{
		UpdateLog();
		time_t timep;
		struct tm *p;
		timep=time(NULL);
		p=localtime(&timep);
		char day1[128];
		sprintf(day1,"%4.4d%s%2.2d%s%2.2d",(1900+p->tm_year),"-",(1+p->tm_mon),"-",p->tm_mday);	
		int day;
		LICENCE::getRunDays(day);
		LICENCE::setNowDays(day1,day+1);
		s_Timer = 24;
	}
	else
	{
		s_Timer--;
	}
}

int LICENCE::licence()
{
	string NowTime;
	char curday[128];
	LICENCE::getNowDays(NowTime);	
	time_t timep;
	struct tm *p;
	timep=time(NULL);
	p=localtime(&timep);
	sprintf(curday,"%4.4d%s%2.2d%s%2.2d",(1900+p->tm_year),"-",(1+p->tm_mon),"-",p->tm_mday);	
	PRINT("curday %s",curday);
	PRINT("nowtime %s",NowTime.c_str());
	LICENCE::init();
	int no = UTIL::DaysBetween2Days(NowTime,curday);
	if( no != -1)
	{
		int day;
		LICENCE::getRunDays(day);
		LICENCE::setNowDays(curday,day+no);	
	}
	else
	{
		system("ntpdate ntp.api.bz");
		p=localtime(&timep);	
		sprintf(curday,"%4.4d%s%2.2d%s%2.2d",(1900+p->tm_year),"-",(1+p->tm_mon),"-",p->tm_mday);		
		no = UTIL::DaysBetween2Days(NowTime,curday);
		if( no != -1)
		{
			int day;
			LICENCE::getRunDays(day);
		  	LICENCE::setNowDays(curday,day+no);
		}
		else
		{
			TimeError = true;
		}
	}
	s_Timer = 24;
	struct itimerval tick;    
    signal(SIGALRM, addDays);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 1;
    tick.it_value.tv_usec = 0;
    tick.it_interval.tv_sec = 3600;
    tick.it_interval.tv_usec = 0;
    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
         ERROR("Set timer failed!");
	return 1;	
}

void LICENCE::init()
{
	pthread_mutex_init(&file_Mutex, NULL);	
}

void LICENCE::destroy()
{
	pthread_mutex_destroy(&file_Mutex);	
}

bool LICENCE::getTimeError()
{
	return TimeError;
}
int LICENCE::getRunDays(int &RunDays)
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(outBuf.c_str(),root))
	{
		RunDays = root["RUNDAYS"].asInt();
		return 1;
	}
	return 0;
}
int LICENCE::getNowDays(string &NowTime)
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(outBuf.c_str(),root))
	{
		NowTime = root["NOWTIME"].asString();
		return 1;
	}
	return 0;
}
int LICENCE::setNowDays(string NowTime,int RunDays)
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	Json::Value tmp;
	if(reader.parse(outBuf.c_str(),root))
	{
		tmp["DAYS"] = root["DAYS"].asInt();
		tmp["EXPIRETIME"] = root["EXPIRETIME"].asString();
		tmp["NOWTIME"] = NowTime.c_str();
		tmp["PTHREADMAX"] = root["PTHREADMAX"].asInt();
		tmp["REGISTERTIME"] = root["REGISTERTIME"].asString();
		tmp["RUNDAYS"] = RunDays;		
		INFO("license expiration date is %s, left %d days",root["EXPIRETIME"].asString().c_str(),root["DAYS"].asInt()-RunDays);
		pthread_mutex_lock(&file_Mutex);
		encrypt(tmp.toStyledString());
		pthread_mutex_unlock(&file_Mutex);
		return 1;
	}
	return 0;
}
int LICENCE::checkTime()
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(outBuf.c_str(),root))
	{	
		string ExpireTime = root["EXPIRETIME"].asString();
		string NowTime = root["NOWTIME"].asString();
		int tmp = UTIL::DaysBetween2Days(NowTime,ExpireTime);
		if(tmp == -1)
			return false;
		return true;
	}
	return -1;
}
int LICENCE::getTime(int &RunDays,int &SurplusDays,string &ExpireTime)
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(outBuf.c_str(),root))
	{	
	 	RunDays = root["RUNDAYS"].asInt();
		SurplusDays = root["DAYS"].asInt()-RunDays;
		if(SurplusDays < 0)
			SurplusDays = -1;
		ExpireTime = root["EXPIRETIME"].asString();		
		return 1;
	}
	return 0;
}
int LICENCE::getPthreadNum()
{
	string outBuf;
	pthread_mutex_lock(&file_Mutex);
	decrypt(outBuf);
	pthread_mutex_unlock(&file_Mutex);
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(outBuf.c_str(),root))
	{
		return root["PTHREADMAX"].asInt();
	}
	return 0;
}







