#include "dispath/taskdispath.h"
#include "util/util.h"
#include "util/simple_log.h"
#include "http/http_client.h"
#include "util/errorcode.h"

//extern CTS_DB *g_db;

TaskDispath::TaskDispath()
{
	pthread_mutex_init(&dis_mutex, NULL);
	pthread_mutex_init(&seed_mutex, NULL);
	pthread_mutex_init(&assign_mutex, NULL);
	
}
TaskDispath::~TaskDispath()
{
	pthread_mutex_destroy(&dis_mutex);
	pthread_mutex_destroy(&seed_mutex);
	pthread_mutex_destroy(&assign_mutex);
}
int TaskDispath::init(string opt)
{
	list<SeedType>::iterator iter;
	SeedType seed;
	pthread_mutex_lock(&seed_mutex);
	for(iter=this->_seed_ser.begin();iter!=this->_seed_ser.end();iter++)
	{
		if(strcmp(opt.c_str(),iter->seedinfo.c_str()) == 0)
		{
			pthread_mutex_unlock(&seed_mutex);
			return iter->seedflag;
		}
	}
	seed.seedinfo = opt;
	seed.seedflag = this->_seed_ser.size();
	seed.num = 0;
	this->_seed_ser.push_back(seed);
	//CTS_DB::addSeedTypeData(seed.seedinfo,seed.seedflag,seed.num);
	pthread_mutex_unlock(&seed_mutex);
	return seed.seedflag;
}
void TaskDispath::watchSeedService()
{
	while(1)
	{
		DEBUG("watch seed service!");
		this->delDispath();
		sleep(5);
	}
}

string TaskDispath::getDispathNodeInfo(int flag)
{
	int use = 0;
	list<DispathNode>::iterator iter;
	DispathNode node;
	string seedinfo;
	pthread_mutex_lock(&assign_mutex);
	if(this->_dispath_node.size() == 0)
	{
		pthread_mutex_unlock(&assign_mutex);
		return seedinfo;
	}
	for(iter=this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(flag == iter->getUseFlag())
		{
			DEBUG("seed info %s,max_use:%d,cur_use:%d",iter->getSeedInfo().c_str(),iter->getMaxUse(),iter->getCurUse());
			if(iter->getExist() == SEED_LIVE &&  use < iter->isUse())
			{
				use = iter->isUse();
				seedinfo = iter->getSeedInfo();
			}
		}
	}
	pthread_mutex_unlock(&assign_mutex);
	return seedinfo;
}
int TaskDispath::acquireDistpathNode(string seedinfo,int flag)
{
	list<DispathNode>::iterator iter;
	pthread_mutex_lock(&assign_mutex);
	for(iter=this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(iter->getUseFlag() == flag)
		{
			if(iter->getExist() == SEED_LIVE && strcmp(seedinfo.c_str(),iter->getSeedInfo().c_str()) == 0)
			{
				iter->inCurUse();
				//CTS_DB::setSeedServiceData(seedinfo,iter->getCurUse());
				DEBUG("acquireDistpathNode %s[%d]",seedinfo.c_str(),iter->getCurUse());
				pthread_mutex_unlock(&assign_mutex);
				return SUCCESS;
			}
		}
	}
	pthread_mutex_unlock(&assign_mutex);
	return SERVER_UNEXIST;
}
int TaskDispath::releaseDistpathNode(string seedinfo,int flag)
{
	list<DispathNode>::iterator iter;
	pthread_mutex_lock(&assign_mutex);
	for(iter=this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(iter->getUseFlag() == flag)
		{
			if(iter->getExist() == SEED_LIVE && strcmp(seedinfo.c_str(),iter->getSeedInfo().c_str()) == 0)
			{
				iter->unCurUse();
				DEBUG("releaseDistpathNode %s[%d]",seedinfo.c_str(),iter->getCurUse());
				//CTS_DB::setSeedServiceData(seedinfo,iter->getCurUse());
				pthread_mutex_unlock(&assign_mutex);
				return SUCCESS;
			}
		}
	}
	pthread_mutex_unlock(&assign_mutex);
	DEBUG("finished put Distpath Node!");
	return SERVER_UNEXIST;
}
bool TaskDispath::checkService(string seedinfo,int flag)
{
	list<DispathNode>::iterator iter;
	for(iter=this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(strcmp(seedinfo.c_str(),iter->getSeedInfo().c_str())==0 && flag == iter->getUseFlag())
			return true;
	}
	return false;
}
void TaskDispath::updateSeedService(int flag,bool stat)
{
	list<SeedType>::iterator iter;
	SeedType seed;
	pthread_mutex_lock(&seed_mutex);
	for(iter=this->_seed_ser.begin();iter!=this->_seed_ser.end();iter++)
	{
		if(iter->seedflag == flag)
		{
			if(stat)
				iter->num++;
			else
				iter->num--;
			//CTS_DB::setSeedTypeData(iter->seedinfo,iter->num);
		}
	}
	pthread_mutex_unlock(&seed_mutex);
}
int TaskDispath::addDispath(string seedinfo,int max_use,int cur_use,int flag)
{
	list<DispathNode>::iterator iter;
	DispathNode node(seedinfo,max_use,cur_use,flag);
	pthread_mutex_lock(&dis_mutex);
	for(iter=this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(strcmp(seedinfo.c_str(),iter->getSeedInfo().c_str()) == 0)
		{
			INFO("seed service register again!");
			pthread_mutex_unlock(&dis_mutex);
			return SERVER_EXIST;
		}
	}
	this->_dispath_node.push_back(node);
	updateSeedService(flag,true);
	//CTS_DB::addSeedServiceData(seedinfo,max_use,cur_use,flag);
	pthread_mutex_unlock(&dis_mutex);
	DEBUG("finished add Dispath!");
	return SUCCESS;
}
int TaskDispath::addDispath(DispathNode node)
{
	pthread_mutex_lock(&dis_mutex);
	node.setExist(SEED_LIVE);
	this->_dispath_node.push_back(node);
	pthread_mutex_unlock(&dis_mutex);
	return 0;
}
int TaskDispath::addSeed(SeedType seed)
{
	pthread_mutex_lock(&seed_mutex);
	this->_seed_ser.push_back(seed);
	pthread_mutex_unlock(&seed_mutex);
	return 0;
}
int TaskDispath::delDispath()
{
	int ret = 0;
	list<DispathNode>::iterator iter;

	for(iter = this->_dispath_node.begin();iter!=this->_dispath_node.end();iter++)
	{
		if(iter->getExist() != SEED_DEAD)
		ret = this->postHeartBeat(iter->getSeedInfo().c_str());
		if(0 != ret)
		{
			DEBUG("post heart beat [%d]",ret);
			sleep(10);
			iter->alterExist();
			if(iter->getExist() == SEED_DEAD)
			{
				pthread_mutex_lock(&dis_mutex);
				//CTS_DB::delSeedServiceData(iter->getSeedInfo());
				updateSeedService(iter->getUseFlag(),false);
				iter = this->_dispath_node.erase(iter);
				pthread_mutex_unlock(&dis_mutex);
			}
		}
		else
			iter->setExist(SEED_LIVE);
	}
	return 0;
}
int TaskDispath::postHeartBeat(const char *seedinfo)
{
	CHttpClient tmp;
	string response;
	char httpath[128];
	memset(httpath,0,sizeof(httpath));
	sprintf(httpath,"%s/status",seedinfo);
	string cmd;
	return tmp.Post(httpath,cmd,response);
}
