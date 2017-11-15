
#include"control/oss_ftp_file.h"
#include <iostream>
#include <string.h>
using namespace std;

int main(int args,char **argv)
{
	OSS_FTP_FILE *oss_file = NULL;
	char src_file[] ="/opt/seed/400/0004.mp4";
	
	oss_file = new OSS_FTP_FILE(argv[1]);
	oss_file->put_object_from_file("400/0004.mp4",src_file);

	delete oss_file;
	return 0;
}







