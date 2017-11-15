#include "control/cts_taskproc.h"
#include "util/cts_util.h"
#include "util/cts_log.h"

CTS_TaskProc::CTS_TaskProc():duration(1),frames(0),alter(0),threadStat(false)
{
}

CTS_TaskProc::~CTS_TaskProc()
{
}

void CTS_TaskProc::parse(string id)
{	
	this->_id = id;
}
int CTS_TaskProc::isComplete()
{
	if(this->duration <= this->frames)
		return 1;
	else
		return 0;
}
bool CTS_TaskProc::getTheadUpdateStates()
{
	return this->threadStat;
}
void CTS_TaskProc::setThreadUpdateStates()
{
	this->threadStat = true;
}
int CTS_TaskProc::UpdateProc(const char *httpath)
{	
	DEBUG("update proc %s",this->_id.c_str());
	CTS_CHttpClient tmp;
	Json::Value arrayItem;
	string response;
	arrayItem["task_id"] = this->getId();
	arrayItem["duration"] = this->duration;
	arrayItem["frames"] = this->frames;
	string cmd("content=");
	cmd.append(arrayItem.toStyledString());
	tmp.Post(httpath,cmd,response);
	DEBUG("%s%s",httpath,cmd.c_str());
	return isComplete();
}

void CTS_TaskProc::setAlter(int al)
{
	this->alter = al;
}
int CTS_TaskProc::getAlter()
{
	return this->alter;
}
bool CTS_TaskProc::isAlter()
{
	if(this->duration == 1 || this->alter != this->frames)
	{
		this->alter = this->frames;
		return true;
	}
	else
		return false;
}

void CTS_TaskProc::setFrames(int frame)
{
	this->frames = frame;
}

void CTS_TaskProc::setDuration(int dur)
{
	this->duration = dur;
}
const char * CTS_TaskProc::getId()
{
	return _id.c_str();
}
void CTS_TaskProc::setId(char * id)
{
	this->_id = id;
	return;
}

