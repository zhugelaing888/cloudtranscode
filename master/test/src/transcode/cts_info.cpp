#include "transcode/cts_info.h"
#include "util/simple_log.h"
#include "util/errorcode.h"
#include "http/http_client.h"
#include "json/json.h"


void rapidxml::parse_error_handler(const char *what, void *where)
{
	return ;
}

CTS_Info::CTS_Info():_id(NULL),
	_progress(NULL),
	_result(NULL),
	_priority(0),
	ackstat(false),
	dealstat(UNDEALING),
	ackcode(0)
{
}

CTS_Info::CTS_Info(const char *path):_id(NULL),
	_progress(NULL),
	_result(NULL),
	_priority(0),
	ackstat(false),
	dealstat(UNDEALING),
	ackcode(0)
{
	memcpy(this->_path,path,strlen(path)+1);
}
CTS_Info::CTS_Info(const char *task_id,const char *result,const char *filename,const int ecode):
	_priority(0),
	ackstat(false),
	dealstat(UNDEALING),
	ackcode(ecode)
{
	this->_progress = NULL;
	this->_id = new char[strlen(task_id)+1];
	memcpy(this->_id,task_id,strlen(task_id)+1);
	this->_result = new char[strlen(result)+1];
	memcpy(this->_result,result,strlen(result)+1);
	this->_taskack.parse(this->_id);
	if(filename)
		memcpy(this->_filename,filename,strlen(filename)+1);
}

CTS_Info::~CTS_Info()
{
	release();
}

void CTS_Info::setPath(char *path)
{
	memcpy(this->_path,path,strlen(path)+1);
	return;
}
char * CTS_Info::getFilename()
{
	return this->_filename;
}
void CTS_Info::setFilename(char *task_id)
{
	sprintf(this->_filename,"%s/%s.xml",this->_path,task_id);
	return;
}
void CTS_Info::rmFilename()
{	
	if(access(this->_filename,0) == 0)
	{
	  	remove(this->_filename);
	}
}

void CTS_Info::recordSeedInfo(string seedinfo)
{
	this->_seedinfo = seedinfo;
}
string CTS_Info::getSeedInfo()
{
	return this->_seedinfo;
}

int CTS_Info::postCelTask()
{
	CHttpClient tmp;
	string response;
	char httpath[128];
	memset(httpath,0,sizeof(httpath));
	sprintf(httpath,"http://%s/status",this->_seedinfo.c_str());
	
	string cmd("content=");
	cmd.append(this->_id);
	return tmp.Post(httpath,cmd,response);
}
int CTS_Info::postProgress()
{
	CHttpClient tmp;
	Json::Value arrayItem;
	string response;
	arrayItem["task_id"] = this->getId();
	arrayItem["duration"] = 1;
	arrayItem["frames"] = 0;
	string cmd("content=");
	cmd.append(arrayItem.toStyledString());
	tmp.Post(this->getProgress(),cmd,response);
	INFO("%s%s",this->getProgress(),cmd.c_str());
	return 0;
}

int CTS_Info::parseAll(rapidxml::xml_document<> *doc)
{
  INFO("begin parse the task info!");
  rapidxml::xml_node<> *root = doc->first_node();   
  rapidxml::xml_node<> *id = root->first_node("id");
  if(!id || id->value_size() == 0)
  {
   	ERROR("the task id is NULL ");
		return TASK_FORMAT_NULL;
  }
  else
  	this->setId(id->value());
  rapidxml::xml_node<> *proc = root->first_node("progress");
  if(!proc || proc->value_size()==0)
  {
   	ERROR("the task progress is NULL");
		return TASK_FORMAT_NULL;
  }
  else
  	this->setProgress(proc->value());
  rapidxml::xml_node<> *result = root->first_node("result");
  if(!result || result->value_size()==0 )
  {
   	ERROR("the task result is NULL");
		return TASK_FORMAT_NULL;
  }
  else
 	 this->setResult(result->value()); 
  rapidxml::xml_node<> *priority = root->first_node("priority");
  if(!priority || priority->value_size()==0)
  	this->setPriority(0);
  else
 	 this->setPriority(atoi(priority->value()));
  rapidxml::xml_node<> *in = root->first_node("input");
  if(!in) 
  	return TASK_FORMAT;
  rapidxml::xml_node<> *infile = in->first_node("file");
  if(infile)
  {		
		rapidxml::xml_node<> *path = infile->first_node("path");
		if(!path || path->value_size()==0)
		{
			ERROR("the task infile path is NULL!");
			return TASK_FORMAT_NULL;
		}
  }
  else
  	return TASK_FORMAT;
  rapidxml::xml_node<> *out = root->first_node("output");
  if(!out)
  	return TASK_FORMAT;
  for(rapidxml::xml_node<> *oufile = out->first_node("file");oufile;oufile = oufile->next_sibling("file"))
  {
		if(!oufile)return -1;
		rapidxml::xml_node<> *out_path = oufile->first_node("out_path");
		if(!out_path || out_path->value_size()==0)
		{
			ERROR("the outfile out_path is NULL!");
			return TASK_FORMAT_NULL;
		}
		rapidxml::xml_node<> *video = oufile->first_node("video");
		if(!video)
			return TASK_FORMAT;
	  rapidxml::xml_node<> *audio = oufile->first_node("audio");
		if(!audio)
			return TASK_FORMAT;
  }
  this->_taskack.parse(this->_id);  
  this->setFilename(this->_id);
  if(access(this->_filename,0) != 0)
  {
  	std::ofstream out(this->_filename);  
		out << (*doc);
  }
  INFO("end the task info parse! task id :%s",this->_id);
  return SUCCESS;
}
int CTS_Info::UpdateAck()
{
	int ret = 0;
	DEBUG("begin update ack info! task id :%s<%s>",this->_id,this->_result);
	
	if(this->_result == NULL)
	{
		WARN("result is NULL!");
		return -1;
	}
	ret = this->_taskack.UpdateAck(this->getResult(),this->ackcode);
	if(ret == 0)
	{
		INFO("sucess update ack info! task id :%s",this->_id);
		this->setAckStatus();
	}
	this->rmFilename();
	return ret;
}

void CTS_Info::setDealStatus(int stat)
{
	this->dealstat = stat;
}
int CTS_Info::isDealStatus()
{
	return this->dealstat;
}


void CTS_Info::setAckStatus()
{
	this->ackstat = true;
}
bool CTS_Info::isAckStatus()
{
	return this->ackstat;
}

char * CTS_Info::getId()
{
	return _id;
}
void CTS_Info::setId(char * id)
{
	if(this->_id == NULL)
		this->_id = new char[strlen(id)+1];
	memcpy(_id,id,strlen(id)+1);
	return ;
}

char * CTS_Info::getResult()
{
	return _result;
}
void CTS_Info::setResult(char * result)
{
	if(this->_result == NULL)
		this->_result = new char[strlen(result)+1];
	memcpy(this->_result,result,strlen(result)+1);
	return;
}
char * CTS_Info::getProgress()
{
	return _progress;
}
void CTS_Info::setProgress(const char * progress)
{
	if(this->_progress == NULL)
		this->_progress = new char[strlen(progress)+1];
	memcpy(this->_progress,progress,strlen(progress)+1);
	return;
}

int CTS_Info::getPriority()
{
	return _priority;
}
void CTS_Info::setPriority(int priority)
{
	_priority = priority;
	return;
}
int CTS_Info::getAckCode()
{
	return this->ackcode;
}
void CTS_Info::setAckCode(int ecode)
{
	this->ackcode = ecode;
}

void CTS_Info::release()
{
	if(this->_id)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
	if(this->_result)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
	return;
}

