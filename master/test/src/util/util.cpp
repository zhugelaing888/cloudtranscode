#include "util/util.h"
#include "util/simple_log.h"

static bool IsLeap(int year);
static bool StringToDate(string date, int& year, int& month, int& day);
static int DayInYear(int year, int month, int day);

static bool IsLeap(int year)
{
   return (year % 4 ==0 || year % 400 ==0) && (year % 100 !=0);
}
static bool StringToDate(string date, int& year, int& month, int& day)
{
    year = atoi((date.substr(0,4)).c_str());
    month = atoi((date.substr(5,2)).c_str());
    day = atoi((date.substr(8,2)).c_str());
    int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(IsLeap(year)){
         DAY[1] = 29;
    }
    return year >= 0 && month<=12 && month>0 && day<=DAY[month-1] && day>0;
}

static int DayInYear(int year, int month, int day)
{
    //int _day = 0;
    int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(IsLeap(year))
        DAY[1] = 29;
    for(int i=0; i<month - 1; ++i)
    {
        day += DAY[i];
    }
    return day;
}

int UTIL::DaysBetween2Days(string date1, string date2)
{
     //取出日期中的年月日
     int year1, month1, day1;
     int year2, month2, day2;
     if(!StringToDate(date1, year1, month1, day1) || !StringToDate(date2, year2,month2,day2))
     {
        cout<<"error"<<endl;
        return -1;
     }
     if(year1 == year2 && month1 == month2)
     {
          return day1 > day2 ? -1 : day2 - day1;
      
     //如果年相同
     }else if(year1 == year2)
     {
         int d1, d2;
         d1 = DayInYear(year1, month1, day1);
         d2 = DayInYear(year2, month2, day2);
         return d1 > d2 ? -1 : d2 - d1;
      
     //年月都不相同
     }else{
         //确保year1年份比year2早
         if(year1 > year2)
         {
         		return -1;
         }
         int d1,d2,d3;
         if(IsLeap(year1))
             d1 = 366 - DayInYear(year1,month1, day1); //取得这个日期在该年还于下多少天
         else
             d1 = 365 - DayInYear(year1,month1, day1);
         d2 = DayInYear(year2,month2,day2); //取得在当年中的第几天
          
         d3 = 0;
         for(int year = year1 + 1; year < year2; year++)
         {
             if(IsLeap(year))
                 d3 += 366;
             else
                 d3 += 365;
         }
         return d1 + d2 + d3;
     }
}

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

string UTIL::getlocaltime()
{
	time_t timep;
	struct tm * p;
	char str[20];

	time(&timep);
	p = localtime(&timep);
	
	sprintf(str, "%4d%02d%02d/%02d%02d%02d_", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	return str;
}



