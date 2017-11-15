#ifndef CTS_TASKQUEUE_H__
#define CTS_TASKQUEUE_H__

#include<list>
#include<iostream>
#include<string.h>
#include <fstream>
#include <sstream>

#include "control/cts_taskinfo.h"
#include "util/cts_db.h"

using namespace std;

class CTS_TaskInfo;



class CTS_TaskQueue
{
public:
	CTS_TaskQueue();
	~CTS_TaskQueue();	
	int addTask(CTS_TaskInfo*  task);
	int celTask(const char *id);	
	void delTask(const char *id);
	int getsize();
	void EraseList();
public:	
	list<cts_ack> acklist; 
private:
	pthread_mutex_t m_mutex;
	char ackfile[128];
	list<CTS_TaskInfo *> dealtask;
};



#endif //__TASKQUEUE_H__
