#ifndef LICENCE_H_
#define LICENCE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h> 
#include <stdarg.h>
const int SIZE = 1024;
using namespace std;



namespace LICENCE
{
	int licence();
	void init();
	int getRunDays(int &RunDays);
	int getNowDays(string &NowTime);
	int setNowDays(string NowTime,int RunDays);
	int checkTime();
	int getTime(int &RunDays,int &SurplusDays,string &ExpireTime);
	int getPthreadNum();
	void destroy();
	bool getTimeError();
};


#endif //LICENCE_H_