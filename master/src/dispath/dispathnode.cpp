#include"dispath/dispathnode.h"
#include "util/simple_log.h"

DispathNode::DispathNode():isexist(SEED_LIVE)
{
	pthread_mutex_init(&use_mutex, NULL);
}
DispathNode::DispathNode(string seedinfo,int maxuse,int curuse,int flag):
_seedinfo(seedinfo),
max_use(maxuse),
cur_use(curuse),
useflag(flag),
isexist(SEED_LIVE)
{
	pthread_mutex_init(&use_mutex, NULL);
}
DispathNode::~DispathNode()
{
	pthread_mutex_destroy(&use_mutex);
}

string DispathNode::getSeedInfo()
{
	return this->_seedinfo;
}
void DispathNode::setSeedInfo(string seedinfo)
{
	this->_seedinfo = seedinfo;
}

void DispathNode::setUseFlag(int flag)
{
	this->useflag = flag;
}
int DispathNode::getUseFlag()
{
	return this->useflag;
}
void DispathNode::setExist(int state)
{
	this->isexist = state;
}
void DispathNode::alterExist()
{
	this->isexist--;
}
int DispathNode::getExist()
{
	return this->isexist;
}
void DispathNode::setMaxUse(int use)
{
	this->max_use = use;
	return;
}
int DispathNode::getMaxUse()
{
	return this->max_use;
}
int DispathNode::getCurUse()
{
		return this->cur_use;
}
int DispathNode::isUse()
{
	pthread_mutex_lock(&use_mutex);
	int ret = (this->cur_use) <= (this->max_use) ? (this->max_use)-(this->cur_use):-1;
	pthread_mutex_unlock(&use_mutex);
	return ret;
}

void DispathNode::setCurUse(int use)
{
	pthread_mutex_lock(&use_mutex);
	this->cur_use = use;
	pthread_mutex_unlock(&use_mutex);
}
void DispathNode::inCurUse()
{
	pthread_mutex_lock(&use_mutex);
	this->cur_use++;
	pthread_mutex_unlock(&use_mutex);
}
void DispathNode::unCurUse()
{
	pthread_mutex_lock(&use_mutex);
	this->cur_use--;
	pthread_mutex_unlock(&use_mutex);
}
