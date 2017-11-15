#include "control/cts_transerv.h"
#include "control/cts_taskack.h"
#include "code/cts_decode.h"
#include "code/cts_encode.h"
#include "code/cts_qt_faststart.h"
#include "util/cts_log.h"
#include "util/cts_util.h"
#include "control/oss_ftp_file.h"

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <libgen.h>


using namespace std;

extern string g_tomcat_path;
extern OSS_FTP_FILE * g_oss_ftp;


CTS_Transerv::CTS_Transerv()
{
}

CTS_Transerv::~CTS_Transerv()
{
}

int CTS_Transerv::TransCode(CTS_TaskInfo *taskinfo)
{
	int ret = 0;
	int ii = 1;
	list<CTS_OutFile*>::iterator oter;
	string input_file;
	//if(taskinfo->ossflag == 1)
	input_file.append(g_tomcat_path);
	input_file.append("/").append(taskinfo->_infile.getPath());
	const int num = taskinfo->getOutfileSize();
	CTS_decode *input_video = new CTS_decode();	
	INFO("task id %s : input video [%s]",taskinfo->getId(),taskinfo->_infile.getPath());
	if(!input_video->dec_init(input_file.c_str()))
	{
		ERROR("task id %s : input video access to information error [%s]",taskinfo->getId(),taskinfo->_infile.getPath());
		delete input_video;
		return 1;
	}
	int duration = input_video->get_duration();
	taskinfo->setDuration(duration);	
	CTS_encode *output_video[num];
	for(oter = taskinfo->_outfile.begin();oter != taskinfo->_outfile.end();oter++)
	{
		output_video[ii-1] = new CTS_encode();
		/*char cmd[100];
		//char *path = dirname((*oter)->getOutPath());
		sprintf(cmd,"%s",(*oter)->getOutPath());
		char cmd1[100];
		sprintf(cmd1,"mkdir -p %s",dirname(cmd));
		system(cmd1);
		*/
		if(!output_video[ii-1]->enc_init(input_video,(*oter)->getCacheFile(),(*oter)->getVideo(),(*oter)->getAudio()))
		{
			ERROR("task id %s : output video initialization error [%s]",taskinfo->getId(),(*oter)->getCacheFile());
			delete input_video;
			for(int j = 0;j < ii-1;j++)
				delete output_video[j];
			return 1;
		}
		ii++;
	}
	while(1)
	{		
		int got_frame = 0;
		if(taskinfo->isCelStatus())
		{
			taskinfo->setAlter(-1);
			for(oter = taskinfo->_outfile.begin();oter != taskinfo->_outfile.end();oter++)
			{
				if(access((*oter)->getCacheFile(),0) == 0)
					remove((*oter)->getCacheFile());
			}
			INFO("cancel transcode task! task id : %s",taskinfo->getId());
			break;
		}		
		AVFrame *frame = av_frame_alloc();		
		if(input_video->dec_get_frame(&frame,&got_frame,&ret) < 0)
		{
			av_frame_free(&frame);
			break;
		}
		if(got_frame)
		{
			for(int j = 0;j < num;j++)
			{
				if(output_video[j]->enc_write_frame(input_video,frame,ret) == 0)
				{
					WARN("task id %s : enc_write_frame error ",taskinfo->getId());
				}
			}
			if(output_video[0]->getPts()%500==0)
			{
				taskinfo->setFrames(output_video[0]->getPts());
				taskinfo->UpdateProc();
			}		
		}
		av_frame_free(&frame);
	}	
	if(!taskinfo->isCelStatus())
	{
		int j = 0;
		for(oter = taskinfo->_outfile.begin();oter != taskinfo->_outfile.end();oter++)
		{
			int upload = 5;
			AVFormatContext *ofmt_ctx = output_video[j]->enc_get_fmt();
			for (unsigned int i = 0; i < ofmt_ctx->nb_streams; i++) 
			{
				output_video[j]->flush_encoder(ofmt_ctx,i);
		    }
			av_write_trailer(ofmt_ctx);	
			INFO("%s qt_faststart start",(*oter)->getCacheFile());			
			char cmd[512];
		    memset(cmd,0,sizeof(cmd));
		   	sprintf(cmd,"%s.bak",(*oter)->getCacheFile());
			ret = qt_faststart((*oter)->getCacheFile(),cmd);
			if(ret == 0)
			{
				char cmd1[512];
				sprintf(cmd1,"yes | mv %s %s",cmd,(*oter)->getOutPath());
				INFO("%s",cmd1);
				system(cmd1);
			}
			else
			{
				ERROR("%s qt_faststart error",(*oter)->getOutPath());
			}
			INFO("task id %s : output video [%s]",taskinfo->getId(),(*oter)->getOutPath());	
			while(upload--)
			{
				if(access((*oter)->getCacheFile(),0) == 0)
				{
					INFO("begin put object from file [%s]!",(*oter)->getOutPath());
					if(g_oss_ftp->put_object_from_file((*oter)->getOutPath(),(*oter)->getCacheFile()) == 0)
					{
						remove((*oter)->getCacheFile());
						break;
					}
					sleep(5);
				}
			}	
			delete output_video[j];
			j++;
		}
		delete input_video;	
	}		
	else
	{		
		delete input_video;
		int j = 0;
		for(oter = taskinfo->_outfile.begin();oter != taskinfo->_outfile.end();oter++)
		{
			char cmd[512];
			sprintf(cmd,"rm -rf %s",(*oter)->getOutPath());
			delete output_video[j];
			j++;
		}
		taskinfo->setAlter(-1);
		return 1;
	}	
	taskinfo->setFrames(duration);
	return 0;
}
