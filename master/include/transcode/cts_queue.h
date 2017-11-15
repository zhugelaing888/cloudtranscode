#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include<list>
#include<iostream>
#include<string.h>
#include <fstream>
#include <sstream>

#include "transcode/cts_info.h"

using namespace std;

class CTS_Info;
	
class CTS_Queue
{
public:
	CTS_Queue();
	~CTS_Queue();

	
	int addTask(CTS_Info*  task,int opt,bool isload=false);
	bool getTask(CTS_Info *&info,int opt);
	int celTask(const char *id,int opt);
	int delTask(const char *id,int opt);
	bool checkId(const char * id);

	void checkDealStats(int opt);
	int addDealTask(CTS_Info*taskinfo);
	void delDealTask(const char *task_id,int opt);
	bool getDealTask(CTS_Info *&taskinfo,int opt);


	void recodeAckTask(const char *task_id,int result,int opt);
	void addAckTask(CTS_Info * taskinfo);
	void delAckTask();
	int getTaskSize();

private:
	CTS_Info * getDealTask(const char *task_id);
	void EraseList();
private:
	list<CTS_Info *> _task;
	list<CTS_Info *> dealtask;
	list<CTS_Info *> acktask;
	pthread_mutex_t task_mutex;
	pthread_mutex_t deal_mutex;
	pthread_mutex_t ack_mutex;
};



#endif //__TASKQUEUE_H__
