#ifndef __TASKACK_H__
#define __TASKACK_H__

#include <iostream>
#include <string.h>
using namespace std;

class CTS_Ack
{
public:
		CTS_Ack();
		~CTS_Ack();
		
		int parse(char *id);
		
		int UpdateAck(const char *httpath,int ecode);
private:
	char *_id;
	
};



#endif //__TASKACK_H__
