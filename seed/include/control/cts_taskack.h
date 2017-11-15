#ifndef CTS_TASKACK_H__
#define CTS_TASKACK_H__


#include <list>
#include <iostream>
#include <string.h>
#include <string>
#include "http/cts_http_client.h"

using namespace std;


class CTS_TaskAck
{
public:
		CTS_TaskAck();
		~CTS_TaskAck();
		
		int parse(string id);
		string getId();
		
		int UpdateUserAck(const std::string httpath,int ecode);
		int UpdateMasterAck(const std::string httpath,int stat,int ecode);
			
private:
	string _id;	
};



#endif //__TASKACK_H__
