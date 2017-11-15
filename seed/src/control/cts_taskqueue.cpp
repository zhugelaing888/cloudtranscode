#include "control/cts_taskqueue.h"
#include "util/cts_log.h"

extern pthread_mutex_t m_Mutex;

CTS_TaskQueue::CTS_TaskQueue()
{
	pthread_mutex_init(&m_mutex, NULL);
}

CTS_TaskQueue::~CTS_TaskQueue()
{
	if(dealtask.size() != 0)
	{
		EraseList();
	}
	pthread_mutex_destroy(&m_mutex);
}
void CTS_TaskQueue::EraseList()
{
	CTS_TaskInfo * taskinfo = NULL;
	list<CTS_TaskInfo *>::iterator iter;
	for(iter = dealtask.begin();iter != dealtask.end();iter++)
	{
		taskinfo = *iter;
		dealtask.erase(iter);
		delete taskinfo;
		taskinfo = NULL;
	}
}
void CTS_TaskQueue::delTask(const char *id)
{
	CTS_TaskInfo * taskinfo = NULL;
	list<CTS_TaskInfo *>::iterator iter;
	pthread_mutex_lock(&m_mutex);
	for(iter = dealtask.begin();iter != dealtask.end();iter++)
	{
		if(!strcmp((*iter)->getId(),id))
		{
			taskinfo = *iter;
			DEBUG("Remove the finished task %s!",taskinfo->getId());
			iter = dealtask.erase(iter);
			if(taskinfo)
			{
				delete taskinfo;
				taskinfo = NULL;
			}
			break;
		}
	}
	pthread_mutex_unlock(&m_mutex);
}
int CTS_TaskQueue::addTask(CTS_TaskInfo* task)
{
	DEBUG("add Task begin!");
	pthread_mutex_lock(&m_mutex);
	DEBUG("1add Task begin!");
	dealtask.push_back(task);
//	task->UpdateProc();
	pthread_mutex_unlock(&m_mutex);
	return 0;
}

int CTS_TaskQueue::celTask(const char *id)
{
	list<CTS_TaskInfo*>::iterator iter;
	pthread_mutex_lock(&m_mutex);
	for(iter = dealtask.begin();iter != dealtask.end();iter++)
	{
		if(strcmp((*iter)->getId(),id) == 0)
		{
			(*iter)->setCelStatus();			
			pthread_mutex_unlock(&m_mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&m_mutex);
	return -1;
}


int CTS_TaskQueue::getsize()
{
	return dealtask.size();
}