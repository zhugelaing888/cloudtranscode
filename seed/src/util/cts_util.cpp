#include "util/cts_util.h"
#include <string.h>
#include<iostream>
using namespace std;


unsigned long UTIL::getFileSize(const char *filename)  
{  
    struct stat buf;  
    if(stat(filename, &buf)<0)  
    {  
        return 0;  
    }  
    return (unsigned long)buf.st_size;  
} 


char * UTIL::ObtainPath(char *path,char *filename)
{
	int j = 0;
	int len = strlen(path)+1;
	for (int i = 0;i < len;i ++)  
    {  
        if (path[i] == '\\' || path[i] == '/')
			j = i; 
		else if(path[i] == '\0')
		{
			path[j] = '\0';
			strcpy(filename,&path[j+1]);
		}
    }
	return path;
}
int UTIL::CreatDir(const char *pDir)  
{  
    int i = 0;  
    int iRet;  
    int iLen;  
    char* pszDir;  
  	
    if(NULL == pDir)  
    {  
        return 0;  
    }  
    pszDir = strdup(pDir);  
    iLen = strlen(pszDir); 
	if(pszDir[0] == '/')
		i = 1;
	else
		i=0;
    // 创建中间目录  
    for (;i < iLen;i++)  
    {  
        if (pszDir[i] == '\\' || pszDir[i] == '/')  
        {   
            pszDir[i] = '\0';  
  
            //如果不存在,创建  
            iRet = access(pszDir,0);  
            if (iRet != 0)  
            {  
                iRet = MKDIR(pszDir);  
                if (iRet != 0)  
                {  
                    return -1;  
                }
            }  
            //支持linux,将所有\换成/  
            pszDir[i] = '/';  
        }
    }  
//    iRet = MKDIR(pszDir);  
    free(pszDir);  
    return iRet;  
} 

int UTIL::myitoa(int n,char * str,int radix)
{
  int i=0;
  int j;

  do
  {
  	str[i]= n%radix + '0';
  	i++;
  }while ((n/= radix)!=0);

  for (j=0; j<i/2;j++)
  {
    char t= str[j];
    str[j]= str[i-1-j];
    str[i-1-j]= t;
  }
  str[i]= '\0';

  return 1;
}

std::string UTIL::gettimefile()
{
	std::string file;
	char str[20];
	
	time_t timep;
	struct tm * p;

	time(&timep);
	p = localtime(&timep);
	
	sprintf(str, "%4d%02d%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	
	return file.append(str);
}

