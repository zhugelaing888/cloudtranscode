#ifndef CTS_TRANSERV_H__
#define CTS_TRANSERV_H__

#include "control/cts_taskinfo.h"

class CTS_Transerv
{
public:
	CTS_Transerv();
//	CTS_Transerv(const char *config_path);
	~CTS_Transerv();
	int TransCode(CTS_TaskInfo *taskinfo);
private:
};

#endif //__TRANSERV_H__
