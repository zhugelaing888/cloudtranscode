#ifndef __DISPATH_NODE_H__
#define __DISPATH_NODE_H__

#include<string.h>
#include<iostream>
using namespace std;

#define SEED_DEAD  0x00
#define SEED_WEAK1 0x01
#define SEED_WEAK2 0x02
#define SEED_LIVE  0x03

class DispathNode
{
public:
	DispathNode();
	DispathNode(string seedinfo,int maxuse,int curuse,int flag);
	~DispathNode();
	
public:
	string getSeedInfo();
	void setSeedInfo(string seedinfo);
	
	void setMaxUse(int use);
	int getMaxUse();

	void setCurUse(int use);
	int getCurUse();
	void inCurUse();
	void unCurUse();
	int isUse();

	void setUseFlag(int flag);
	int getUseFlag();

	int getExist();
	void alterExist();
	void setExist(int state);

private:
		string _seedinfo;
		int max_use;
		int cur_use;
		int useflag;
		int isexist;
		pthread_mutex_t use_mutex;
};



#endif //__DISPATH_NODE_H__
