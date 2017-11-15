#ifndef CTS_UTIL_H__
#define CTS_UTIL_H__

#include <stdio.h>  
#include <sys/time.h>  
#include <time.h> 
#include <stdarg.h>  
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define MKDIR(a) mkdir((a),0755)
 
namespace UTIL
{
	unsigned long getFileSize(const char *filename);
	int CreatDir(const char *pDir);
	char *ObtainPath(char *path,char *filename = NULL);
	int myitoa(int n,char * str,int radix);
	std::string gettimefile();
};
#endif //__UTIL_H__

