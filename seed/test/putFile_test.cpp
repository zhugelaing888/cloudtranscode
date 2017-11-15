
#include"control/oss_ftp_file.h"
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc,char **argv)
{
	OSS_FTP_FILE *oss_file = NULL;
	if(argc < 3)
	{
		printf("arg error");
		return -1;
	}
	oss_file = new OSS_FTP_FILE("/opt/seed/conf/transcode.conf");
	oss_file->put_object_from_file(argv[2],argv[1]);

	delete oss_file;
	return 0;
}







