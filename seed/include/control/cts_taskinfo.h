#ifndef CTS_TASKINFO_H__
#define CTS_TASKINFO_H__

#include "control/cts_infile.h"
#include "control/cts_outfile.h"
#include "util/cts_util.h"
#include "control/cts_taskproc.h"
#include "control/cts_taskack.h"
#include "util/cts_config.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;



class CTS_InFile;
class CTS_TaskProc;
class CTS_TaskAck;

class CTS_TaskInfo
{
public:
	CTS_TaskInfo(std::string cache);
	CTS_TaskInfo(const char *task_id,const char *result);
	
	~CTS_TaskInfo();
	
	void setCelStatus();
	bool isCelStatus();
	
	int parseAll(rapidxml::xml_document<> *doc);
	
	
	char * getId();
	void setId(char * id);
	
	char * getProgress();
	void setProgress(char * pro);
	
	char * getResult();
	void setResult(char * result);
	
	int getPriority();
	void setPriority(int priority);
	
	int getOutfileSize();

	int UpdateProc();
	int UpdateAck(const std::string masterServerUrl,int &status,int result);
	void setDuration(int dur);
	void setFrames(int frame);
	void setAlter(int al);
	void setThreadUpdateStates();
private:
	void setPath(char *path);
	void releaseOutFile();
	void release();
	void addOutFile(CTS_OutFile *outfile);
private:
	char *_id;
	char *_progress;
	char *_result;
	int _priority;
	
private:
	
	bool ackstat;
	bool delstat;
	bool celstat;
	std::string cachefile;
	CTS_TaskProc  _taskproc;
	CTS_TaskAck _taskack;
public:
	list<CTS_OutFile*> _outfile;
	CTS_InFile _infile;
	int ossflag;
};



#endif  //__TAKINFO_H__
