#ifndef CTS_TASKPROC_H__
#define CTS_TASKPROC_H__

#include <list>
#include <iostream>
#include <string.h>
#include "http/cts_http_client.h"
#include "json/json.h"

using namespace std;

typedef struct ProcInfo
{
	int duration;
	int frames;
}ProcInfo;

class CTS_TaskProc
{
public:
		CTS_TaskProc();
		~CTS_TaskProc();
		
		void parse(string id);
		int UpdateProc(const char *httpath);

		void setDuration(int dur);
		void setFrames(int frame);
		bool isAlter();
		void setAlter(int al);
		int getAlter();

		bool getTheadUpdateStates();
		void setThreadUpdateStates();
		
		const char *  getId();
		void setId(char* id);
private:
	int isComplete();
private:
	string _id;
	int duration;
	int frames;
	int alter;
	bool threadStat;
};


#endif //__TASKPROC_H__
