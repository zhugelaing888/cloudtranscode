#include "util/cts_db.h"
#include <iostream>
#include <pthread.h>

using namespace std;
pthread_mutex_t db_mutex;

int add(int id)
{
	Driver *driver; 
	Connection*conn;
	Statement *state;
	try {
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1:3306","root", "111111");
		conn->setAutoCommit(false);
		conn->getAutoCommit();
		state = conn->createStatement(); 
    	state->execute("use master");
		string cmd("INSERT INTO test VALUES (");
		char str[20];
		sprintf(str,"%d",id);
		cmd.append(str).append(")");	
		state->execute(cmd.c_str());
		conn->commit();
		driver = NULL;	
		conn->close();
		delete conn;
		delete state;
	} catch (SQLException &e) {
		return -1;
	} catch (std::runtime_error &e) {
		return -1;
	}
	return 1;
}

int del(int id)
{
	Driver *driver; 
	Connection*conn;
	Statement *state;
	try {
		driver = sql::mysql::get_mysql_driver_instance();
		conn = driver->connect("tcp://127.0.0.1:3306","root", "111111");
		conn->setAutoCommit(false);
		conn->getAutoCommit();
		state = conn->createStatement(); 
    	state->execute("use master");
		string cmd("DELETE FROM `test` WHERE id=");
		char str[20];
		sprintf(str,"%d",id);
		cmd.append(str).append("");	
		state->execute(cmd.c_str());
		conn->commit();
		driver = NULL;	
		conn->close();
		delete conn;
		delete state;
	} catch (SQLException &e) {
		return -1;
	} catch (std::runtime_error &e) {
		return -1;
	}
	return 0;
}

void *func(void *arg)
{
	for(int i = 0;i < 50;i++)
	{
		pthread_mutex_lock(&db_mutex);
		del(i);
		pthread_mutex_unlock(&db_mutex);
	}
	return (void*)NULL;
}
void *func1(void*arg)
{
	for(int i=50;i < 100;i++)
	{
			pthread_mutex_lock(&db_mutex);
			del(i);
			pthread_mutex_unlock(&db_mutex);
	}
	return (void*)NULL;

}


int main()
{
	pthread_mutex_init(&db_mutex, NULL);
	pthread_t tids[2];
	for(int i = 0 ;i < 100;i++)
		add(i);
	pthread_create( &tids[0], NULL, func, NULL );
	pthread_create( &tids[1], NULL, func1, NULL );
	pthread_join (tids[0],NULL);
	pthread_join (tids[1],NULL);
	pthread_mutex_destroy(&db_mutex);
/*
	CTS_DB DB;
	DB.addSeedServiceData("192.168.3.145:12345",4,0,1);
	DB.addSeedServiceData("192.168.3.146:12345",4,0,1);
	DB.addSeedServiceData("192.168.3.147:12345",4,0,1);
	DB.addSeedServiceData("192.168.3.148:12345",4,0,1);
	list<cts_seedServerice> tmp;
	DB.getSeedServiceData(tmp);
	list<cts_seedServerice>::iterator iter = tmp.begin();
	for(;iter != tmp.end();iter++)
	{
		cout<<iter->seedinfo<<" "<<iter->max_use<<" "<<iter->cur_use<<" "<<iter->useflag<<endl;
	}
	//DB.delSeedServiceData("192.168.3.145:12345");
	DB.setSeedServiceData("192.168.3.146:12345",2);
*/
	return 0;
	
}
