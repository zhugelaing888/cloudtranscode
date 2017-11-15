#ifndef __OSS_FTP_FILE_H__
#define __OSS_FTP_FILE_H__

#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include <iostream>
#include <string.h>
using namespace std;

class OSS_FTP_FILE
{
public:
		OSS_FTP_FILE();
		OSS_FTP_FILE(const char * config_path);
		~OSS_FTP_FILE();
		
		int put_object_from_file(const char *object_name,const char *filename);
		int get_object_to_local_file(const char *object_name,const char *download_filename);
		int delete_object(const char *object_name);
private:
	  int initOSS(const char * config_path);
	  void init_sample_config(oss_config_t *config, int is_oss_domain);
	  void init_sample_request_options(oss_request_options_t *options, int is_oss_domain);
private:
		string OSS_HOST;
		int OSS_PORT;
		string ACCESS_KEY_ID;
		string ACCESS_KEY_SECRET;
		string BUCKET_NAME;
		string MULTIPART_UPLOAD_FILE_PATH;
private:
		pthread_mutex_t oss_mutex;
};

#endif //__OSS_FTP_FILE_H__
