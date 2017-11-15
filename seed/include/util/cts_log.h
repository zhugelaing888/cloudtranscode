#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <cstring>
#include <string>
#include <iostream>
#include <time.h>  
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
using namespace std;

#define MKDIR(a) mkdir((a),0755)

/*日志级别 - 控制日志显示的级别*/
#define ERROR_LEVEL     0x01 		/*错误*/
#define WARN_LEVEL      0x02		/*警告*/
#define INFO_LEVEL      0x03 		/*提示*/
#define DEBUG_LEVEL     0x04 		/*调试*/

typedef struct
{
	char filedir[128]; 		/*日志文件目录, 包括根目录、子目录*/
	char filename[128]; 		/*日志路径文件名*/
	int log_level;
	pthread_mutex_t m_mutex;
}LOG_FILE;



#define ERROR(format, args...) \
		log_error("%s %s %s(%d): " format,"ERROR", get_show_time().c_str(), __FILE__, __LINE__, ##args);

#define WARN(format, args...) \
		log_warn("%s %s %s(%d): " format,"WARN",  get_show_time().c_str(), __FILE__, __LINE__, ##args);

#define INFO(format, args...) \
		log_info("%s %s %s(%d): " format,"INFO",  get_show_time().c_str(), __FILE__, __LINE__, ##args); 

#define DEBUG(format, args...) \
		log_debug("%s %s %s(%d): " format,"DEBUG", get_show_time().c_str(),__FILE__, __LINE__, ##args);
#define PRINT(format, args...) \
		log_print("%s %s(%d): " format, get_show_time().c_str(),__FILE__, __LINE__, ##args); 

std::string get_show_time();

int log_init(const char *filename,int level = DEBUG_LEVEL);
void log_error(const char *format, ...);
void log_warn(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);
void log_print(const char *format, ...);

#endif
