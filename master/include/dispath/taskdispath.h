#ifndef __TASK_DISPATH_H__
#define __TASK_DISPATH_H__

#include "dispath/dispathnode.h"
#include<list>
#include<string.h>
#include<iostream>
using namespace std;


class DispathNode;

typedef struct SeedType
{
	int seedflag;
	string seedinfo;
	int num;
}SeedType;

class TaskDispath
{
public:
	TaskDispath();
	~TaskDispath();

	int init(string opt);
	void watchSeedService();
	string getDispathNodeInfo(int flag);
	int acquireDistpathNode(string seedinfo,int flag);
	int releaseDistpathNode(string seedinfo,int flag);
	int addDispath(string seedinfo,int maxuse,int curuse,int flag);
	int addDispath(DispathNode node);
	int delDispath();
	int addSeed(SeedType seed);
	bool checkService(string seedinfo,int flag);
private:
	void updateSeedService(int flag,bool stat);
	int postHeartBeat(const char *seedinfo);
private:
	list<SeedType> _seed_ser;
	list<DispathNode> _dispath_node;
	pthread_mutex_t dis_mutex;
	pthread_mutex_t seed_mutex;
	pthread_mutex_t assign_mutex;
};
#endif //__TASK_DISPATH_H__

