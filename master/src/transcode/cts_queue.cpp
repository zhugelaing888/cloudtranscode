#include "transcode/cts_queue.h"
#include "util/simple_log.h"
#include "util/errorcode.h"
#include "dispath/taskdispath.h"

//extern CTS_DB *g_db;
extern TaskDispath *g_dispath;

CTS_Queue::CTS_Queue()
{
	pthread_mutex_init(&task_mutex, NULL);
	pthread_mutex_init(&deal_mutex, NULL);
	pthread_mutex_init(&ack_mutex, NULL);
}

CTS_Queue::~CTS_Queue()
{
	if(dealtask.size() != 0)
	{
		EraseList();
	}
	pthread_mutex_destroy(&task_mutex);
	pthread_mutex_destroy(&deal_mutex);
	pthread_mutex_destroy(&ack_mutex);
}


int CTS_Queue::addTask(CTS_Info* task,int opt,bool isload)  //task
{
	list<CTS_Info*>::iterator iter;
	DEBUG("add Task %s",task->getId());
	if(this->checkId(task->getId()))
		return -1;
	pthread_mutex_lock(&task_mutex);
	for(iter = _task.begin();iter != _task.end();iter++)
	{
		if((*iter)->getPriority() < task->getPriority())
		{
			_task.insert(iter,task);
			if(!isload)
			{
				//CTS_DB::addTransInfoData(task->getId(),task->getFilename(),task->getResult(),task->getPriority(),opt);
				task->postProgress();
			}
			pthread_mutex_unlock(&task_mutex);
			return 0;
		}
	}
	if(!isload)
	{
		//CTS_DB::addTransInfoData(task->getId(),task->getFilename(),task->getResult(),task->getPriority(),opt);
		task->postProgress();
	}
	_task.push_back(task);	
	pthread_mutex_unlock(&task_mutex);
	return 0;
}


bool CTS_Queue::getTask(CTS_Info *&taskinfo,int opt) //task--->dealtask
{
	list<CTS_Info*>::iterator in_iter;
	list<CTS_Info *>::iterator un_iter;
	pthread_mutex_lock(&task_mutex);
	for(in_iter = _task.begin();in_iter != _task.end();in_iter++)
	{
			taskinfo = *in_iter;
			in_iter = _task.erase(in_iter);
			//CTS_DB::delTransInfoData(taskinfo->getId());
			this->dealtask.push_back(taskinfo);
			pthread_mutex_unlock(&task_mutex);
			return true;
	}
	pthread_mutex_unlock(&task_mutex);
	return false;
}
int CTS_Queue::delTask(const char *task_id,int opt)
{
	list<CTS_Info*>::iterator iter;
	CTS_Info *taskinfo = NULL;
	DEBUG("del task trans info %s!",task_id);
	pthread_mutex_lock(&task_mutex);
	for(iter = _task.begin();iter != _task.end();iter++)
	{
		if(strcmp((*iter)->getId(),task_id) == 0)
		{
			taskinfo = *iter;
			iter = _task.erase(iter);
			//CTS_DB::delTransInfoData(taskinfo->getId());
			if(taskinfo)
			{
				taskinfo->rmFilename();
				delete taskinfo;
				taskinfo = NULL;
			}
			pthread_mutex_unlock(&task_mutex);
			return SUCCESS;
		}
	}
	return -1;
}

int CTS_Queue::celTask(const char *id,int opt) //1,task 2,dealtask
{
	CTS_Info *taskinfo = NULL;	
	if(!this->delTask(id,opt))
		return SUCCESS;
	taskinfo = this->getDealTask(id);
	if(!taskinfo)
	{
		WARN("the task has finished transcode!");
		pthread_mutex_unlock(&task_mutex);
		return TASK_FINISHED;
	}
	int ret = taskinfo->postCelTask();
	pthread_mutex_unlock(&task_mutex);
	return ret;
}

int CTS_Queue::addDealTask(CTS_Info*taskinfo)
{
	pthread_mutex_lock(&deal_mutex);
	this->dealtask.push_back(taskinfo);
	pthread_mutex_unlock(&deal_mutex);	
	return 0;
}
void CTS_Queue::checkDealStats(int opt)
{
	list<CTS_Info*>::iterator iter;
	pthread_mutex_lock(&deal_mutex);
	DEBUG("get deal task! task [%d],deal task[%d],ack task[%d]",this->_task.size(),this->dealtask.size(),this->acktask.size());
	for(iter = this->dealtask.begin();iter!=this->dealtask.end();iter++)
	{
		if((*iter)->isDealStatus()==UNDEALING || g_dispath->checkService((*iter)->getSeedInfo(),opt))
			continue;
		else
		{
			(*iter)->setDealStatus(UNDEALING);
			//CTS_DB::setDealInfoData((*iter)->getId(),(*iter)->getSeedInfo(),UNDEALING);
		}
	}
	pthread_mutex_unlock(&deal_mutex);
	return;
}
CTS_Info * CTS_Queue::getDealTask(const char *task_id) // dealtask  celtask
{
	CTS_Info * taskinfo = NULL;
	list<CTS_Info *>::iterator iter;
	pthread_mutex_lock(&deal_mutex);
	for(iter=dealtask.begin();iter!=dealtask.end();iter++)
	{
		if(strcmp((*iter)->getId(),task_id) == 0)
		{
			taskinfo = (*iter);
			pthread_mutex_unlock(&deal_mutex);
			return taskinfo;
		}
	}
	pthread_mutex_unlock(&deal_mutex);
	return NULL;
}
bool CTS_Queue::getDealTask(CTS_Info *&taskinfo,int opt)
{
	list<CTS_Info*>::iterator iter;
	pthread_mutex_lock(&deal_mutex);
	for(iter = this->dealtask.begin();iter!=this->dealtask.end();iter++)
	{
		if(!(*iter)->isDealStatus())
		{
			taskinfo = (*iter);
			if(access(taskinfo->getFilename(),0) != 0)
			{
				iter = this->dealtask.erase(iter);
				continue;
			}
			pthread_mutex_unlock(&deal_mutex);
			return true;
		}
	}
	pthread_mutex_unlock(&deal_mutex);
	return false;
}
void CTS_Queue::delDealTask(const char *task_id,int opt)  //dealtask
{
	CTS_Info * taskinfo = NULL;
	list<CTS_Info *>::iterator iter;
	pthread_mutex_lock(&deal_mutex);
	for(iter = dealtask.begin();iter != dealtask.end();iter++)
	{
		if(strcmp(task_id,(*iter)->getId()) == 0)
		{
			taskinfo = *iter;
			iter = dealtask.erase(iter);
			taskinfo->rmFilename();
			DEBUG("recover dispath deal task %s",task_id);
			g_dispath->releaseDistpathNode(taskinfo->getSeedInfo(),opt);
			//CTS_DB::delDealInfoData(taskinfo->getId());
			if(taskinfo)
			{
				delete taskinfo;
				taskinfo = NULL;
			}
			pthread_mutex_unlock(&deal_mutex);
			return;
		}
	}
	pthread_mutex_unlock(&deal_mutex);
}

void CTS_Queue::recodeAckTask(const char *task_id,int result,int opt)
{
	CTS_Info *taskinfo = NULL;
	list<CTS_Info *>::iterator iter;
	pthread_mutex_lock(&deal_mutex);
	for(iter = this->dealtask.begin();iter != this->dealtask.end();iter++)
	{
		if(strcmp(task_id,(*iter)->getId()) == 0)
		{
			taskinfo = *iter;			
			DEBUG("recover dispath task %s",task_id);
			g_dispath->releaseDistpathNode(taskinfo->getSeedInfo(),opt);
			iter = _task.erase(iter);
			//CTS_DB::delDealInfoData(taskinfo->getId());
			taskinfo->setAckCode(result);
			this->acktask.push_back(taskinfo);
			//CTS_DB::addAckInfoData(taskinfo->getId(),taskinfo->getResult(),opt,result);
			pthread_mutex_unlock(&deal_mutex);
			return;
		}
	}
	pthread_mutex_unlock(&deal_mutex);
}

void CTS_Queue::addAckTask(CTS_Info *taskinfo)
{
	pthread_mutex_lock(&ack_mutex);
	this->acktask.push_back(taskinfo);
	pthread_mutex_unlock(&ack_mutex);	
	return ;
}
void CTS_Queue::delAckTask()
{
	CTS_Info *taskinfo = NULL;
	list<CTS_Info *>::iterator iter;
	pthread_mutex_lock(&ack_mutex);
	for(iter = this->acktask.begin();iter != this->acktask.end();iter++)
	{
		taskinfo = (*iter);
		if(!(*iter)->isAckStatus())
		{
		//	taskinfo->UpdateAck();
		}
		if((*iter)->isAckStatus())
		{
			iter = this->acktask.erase(iter);
			//CTS_DB::delAckInfoData(taskinfo->getId());
			if(!taskinfo)
			{
				delete taskinfo;
			}
		}
		taskinfo = NULL;
	}
	pthread_mutex_unlock(&ack_mutex);
}

bool CTS_Queue::checkId(const char *id)// 1,task 2,dealtask
{
	list<CTS_Info*>::iterator in_iter;
	list<CTS_Info *>::iterator un_iter;
	pthread_mutex_lock(&task_mutex);
	for(in_iter = _task.begin();in_iter != _task.end();in_iter++)
	{
		if(!strcmp((*in_iter)->getId(),id))
		{
			pthread_mutex_unlock(&task_mutex);
			return true;
		}
	}
	for(un_iter = this->dealtask.begin();un_iter != this->dealtask.end();un_iter++)
	{
		if(!strcmp((*un_iter)->getId(),id))
		{
			pthread_mutex_unlock(&task_mutex);
			return true;
		}
	}	
	pthread_mutex_unlock(&task_mutex);
	return false;
}


int CTS_Queue::getTaskSize()
{
	return _task.size();
}

void CTS_Queue::EraseList()
{
	CTS_Info * taskinfo = NULL;
	list<CTS_Info *>::iterator iter;
	for(iter = dealtask.begin();iter != dealtask.end();iter++)
	{
		taskinfo = *iter;
		dealtask.erase(iter);
		delete taskinfo;
		taskinfo = NULL;
	}
}

