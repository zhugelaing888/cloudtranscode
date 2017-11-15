#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>  
#include <sys/time.h>  
#include <time.h> 
#include <stdarg.h>  
#include <sys/stat.h> 
#include <sys/types.h>  
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MKDIR(a) mkdir((a),0755)

#define UNDEALING 0
#define DEALING   1

namespace UTIL
{
	int DaysBetween2Days(string date1, string date2);
	unsigned long getFileSize(const char *filename);
	int CreatDir(const char *pDir);
	char *ObtainPath(char *path,char *filename = NULL);
	int myitoa(int n,char * str,int radix);
	string getlocaltime();
};
#endif //__UTIL_H__

