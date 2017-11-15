#ifndef CTS_DB_H_
#define CTS_DB_H_



#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include "util/cts_log.h"

using namespace std;

#define PATH 512

struct cts_ack
{
	int status;
	int result;
	string task_id;
};

class CTS_DB
{
public:
	CTS_DB();
	~CTS_DB();
private:
};





#endif //CTS_DB_H_
