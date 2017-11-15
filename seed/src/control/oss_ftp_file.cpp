
#include <sys/stat.h>
#include "control/oss_ftp_file.h"
#include "oss_api.h"
#include "util/cts_config.h"
#include "util/cts_log.h"

OSS_FTP_FILE::OSS_FTP_FILE()
{
}
OSS_FTP_FILE::OSS_FTP_FILE(const char *config_path)
{
	pthread_mutex_init(&oss_mutex, NULL);
	this->initOSS(config_path);
}
OSS_FTP_FILE::~OSS_FTP_FILE()
{
	pthread_mutex_destroy(&oss_mutex);
}
int OSS_FTP_FILE::initOSS(const char *config_path)
{
	Config config(config_path);
	OSS_HOST = "oss-cn-beijing-internal.aliyuncs.com";
	OSS_PORT = 80;
	ACCESS_KEY_ID = "9JZSDG2ToR81aSH5";
	ACCESS_KEY_SECRET = "vhvfsl6GeqHH1VGNwVpgFPFiEjJhwG";
	BUCKET_NAME = config.Read("BUCKET_NAME", BUCKET_NAME);
	MULTIPART_UPLOAD_FILE_PATH = "<Please apply your multipart upload file>";
 	/*Config config(config_path);	
	string oss_prot;
	OSS_HOST = config.Read("OSS_HOST", OSS_HOST);
	DEBUG("OSS_HOST %s",OSS_HOST.c_str());	
	oss_prot = config.Read("OSS_PORT", oss_prot);
	OSS_PORT = atoi(oss_prot.c_str());
	DEBUG("OSS_PORT%s %d",oss_prot.c_str(),OSS_PORT);		
	ACCESS_KEY_ID = config.Read("ACCESS_KEY_ID", ACCESS_KEY_ID);
	DEBUG("ACCESS_KEY_ID %s",ACCESS_KEY_ID.c_str());		
	ACCESS_KEY_SECRET = config.Read("ACCESS_KEY_SECRET", ACCESS_KEY_SECRET);
	DEBUG("ACCESS_KEY_SECRET %s",ACCESS_KEY_SECRET.c_str());		
	BUCKET_NAME = config.Read("BUCKET_NAME", BUCKET_NAME);		
	DEBUG("BUCKET_NAME %s",BUCKET_NAME.c_str());		
	MULTIPART_UPLOAD_FILE_PATH = config.Read("MULTIPART_UPLOAD_FILE_PATH", MULTIPART_UPLOAD_FILE_PATH);
	DEBUG("MULTIPART_UPLOAD_FILE_PATH %s",MULTIPART_UPLOAD_FILE_PATH.c_str());*/
	return 0;
}


void OSS_FTP_FILE::init_sample_config(oss_config_t *config, int is_oss_domain)
{
	aos_str_set(&config->endpoint, OSS_HOST.c_str());
    aos_str_set(&config->access_key_id, ACCESS_KEY_ID.c_str());
    aos_str_set(&config->access_key_secret, ACCESS_KEY_SECRET.c_str());
    config->is_cname = is_oss_domain;
}

void OSS_FTP_FILE::init_sample_request_options(oss_request_options_t *options, int is_oss_domain)
{
    options->config = oss_config_create(options->pool);
    init_sample_config(options->config, is_oss_domain);
    options->ctl = aos_http_controller_create(options->pool, 0);
}



int OSS_FTP_FILE::put_object_from_file(const char *object_name,const char *filename)
{
	int ret = 0;
    aos_pool_t *p;
    aos_string_t bucket;
    aos_string_t object;
    int is_oss_domain = 0;
    aos_table_t *headers;
    aos_table_t *resp_headers;
    oss_request_options_t *options;
    aos_status_t *s;
    aos_string_t file;
	
	pthread_mutex_lock(&oss_mutex);
	if (aos_http_io_initialize(filename, 0) != AOSE_OK) 
	{
		pthread_mutex_unlock(&oss_mutex);
      return -1;
  	}
	
    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_oss_domain);
    headers = aos_table_make(options->pool, 1);
	apr_table_set(headers, OSS_CONTENT_TYPE, "video/mp4");
    aos_str_set(&bucket, BUCKET_NAME.c_str());
    aos_str_set(&object, object_name);
    aos_str_set(&file, filename);

    s = oss_put_object_from_file(options, &bucket, &object, &file, headers, &resp_headers);

    if (NULL != s && 2 == s->code / 100) {
        INFO("put %s from %s succeeded",object_name,filename);
    }
    else {
		WARN("put %s from %s failed[%d,%s,%s,%s]",object_name,filename,s->code,s->error_code,s->error_code,s->req_id);
		ret = -1;
    }

    aos_pool_destroy(p);
    aos_http_io_deinitialize();
	pthread_mutex_unlock(&oss_mutex);
	return ret;
}

int OSS_FTP_FILE::get_object_to_local_file(const char *object_name,const char *download_filename )
{
	
	return 0;
}

int OSS_FTP_FILE::delete_object(const char *object_name)
{
	
	return 0;
}
