#ifndef __TASKINFO_H__
#define __TASKINFO_H__

#include "util/util.h"
#include "transcode/cts_ack.h"
#include "util/config.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


class CTS_Ack;

class CTS_Info
{
public:
	CTS_Info();
	CTS_Info(const char *task_id);
	CTS_Info(const char *task_id,const char *result,const char *filename=NULL,const int ecode=0);
	
	~CTS_Info();

	void setAckStatus();
	bool isAckStatus();
	void setDealStatus(int stat);
	int isDealStatus();
	
	int parseAll(rapidxml::xml_document<> *doc);
	
	
	char * getId();
	void setId(char * id);
	
	char * getResult();
	void setResult(char * result);

	char *getProgress();
	void setProgress(const char *progress);
	
	int getPriority();
	void setPriority(int priority);

	int getAckCode();
	void setAckCode(int ecode);

	int UpdateAck();
	
	char *getFilename();
	void rmFilename();
	
	void recordSeedInfo(string seedinfo);
	string getSeedInfo();
	
	int postCelTask();
	int postProgress();
private:
	void setPath(char *path);
	void setFilename(char *task_id);
	void release();
private:
	char *_id;
	char *_progress;
	char *_result;
	int _priority;

	bool ackstat;
	int dealstat;
	int ackcode;
	
private:
	char _path[128];
	char _filename[128];
	string _seedinfo;
	
	CTS_Ack _taskack;
};



#endif  //__TAKINFO_H__
