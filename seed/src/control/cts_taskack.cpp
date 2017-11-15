#include "control/cts_taskack.h"
#include "http/cts_http_client.h"
#include "json/json.h"
#include "util/cts_util.h"
#include "util/cts_log.h"

CTS_TaskAck::CTS_TaskAck()
{
}

CTS_TaskAck::~CTS_TaskAck()
{
}

int CTS_TaskAck::parse(string id)
{
	this->_id = id;
	return 0;
}
string CTS_TaskAck::getId()
{
	return this->_id;
}
int CTS_TaskAck::UpdateUserAck(const std::string httpath,int ecode)
{
	CTS_CHttpClient tmp;
	std::string response;
	std::string cmd("content=");
	Json::Value arrayItem;
	arrayItem["task_id"] = this->getId();
	arrayItem["result"] = ecode;
	cmd.append(arrayItem.toStyledString());
	INFO("update seed ack %s%s",httpath.c_str(),cmd.c_str());	
    return tmp.Post(httpath,cmd,response);
}

int CTS_TaskAck::UpdateMasterAck(const std::string httpath,int stat,int ecode)
{
	CTS_CHttpClient tmp;
	std::string response;
	std::string cmd("content=");
	Json::Value arrayItem;
	arrayItem["task_id"]=this->getId();
	arrayItem["stat"]=stat;
	arrayItem["result"]=ecode;
	cmd.append(arrayItem.toStyledString());
	INFO("update master ack %s%s",httpath.c_str(),cmd.c_str());    
	return tmp.Post(httpath,cmd,response);
}

