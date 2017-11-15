#include "control/cts_outfile.h"
#include "util/cts_log.h"


CTS_OutFile::CTS_OutFile():_name(NULL),_format(NULL),_out_path(NULL),_num(0)
{
	 memset(&_video,0,sizeof(Video));
	 memset(&_audio,0,sizeof(Audio));
	 memset(&_logo,0,sizeof(Logo));
}
CTS_OutFile::~CTS_OutFile()
{
	if(this->_name)
	{
		delete[] this->_name;
		this->_name = NULL;
	}
	if(this->_format)
	{
		delete[] this->_format;
		this->_name = NULL;
	}
	if(this->_out_path)
	{
		delete[] this->_out_path;
		this->_name = NULL;
	}
}

char * CTS_OutFile::getName()
{
	return _name;
}
void CTS_OutFile::setName(char *name)
{
	if(this->_name == NULL)
		this->_name = new char[strlen(name)+1];
	memcpy(_name,name,strlen(name)+1);
	return ;
}
char * CTS_OutFile::getFormat()
{
	return _format;
}
void CTS_OutFile::setFormat(char *format)
{
	if(this->_format == NULL)
		this->_format = new char[strlen(format)+1];
	memcpy(_format,format,strlen(format)+1);
	return;
}

char *CTS_OutFile::getOutPath()
{
	return _out_path;
}
void CTS_OutFile::setOutPath(char *out_path)
{
	if(this->_out_path == NULL)
			this->_out_path = new char[strlen(out_path)+1];
	memcpy(_out_path,out_path,strlen(out_path)+1);
	return;
}
void CTS_OutFile::setCacheFile(std::string cfile)
{
	this->cachefile = cfile;
}
const char * CTS_OutFile::getCacheFile()
{
	return this->cachefile.c_str();
}

int CTS_OutFile::getNum()
{
	return _num;
}
void CTS_OutFile::setNum(int num)
{
	_num = num;
	return ;
}

Video CTS_OutFile::getVideo()
{
	return _video;
}
void CTS_OutFile::setVideo(Video video)
{
	memcpy(this->_video.codec,video.codec,strlen(video.codec)+1);
	memcpy(this->_video.aspect,video.aspect,strlen(video.aspect)+1);
	this->_video.width = video.width;
	this->_video.height = video.height;
	this->_video.bitrate = video.bitrate;
	this->_video.lev = video.lev;
}

Audio CTS_OutFile::getAudio()
{
	return _audio;
}
void CTS_OutFile::setAudio(Audio audio)
{
	memcpy(this->_audio.codec,audio.codec,strlen(audio.codec)+1);
	this->_audio.sample_rate = audio.sample_rate;
	this->_audio.channels = audio.channels;
	this->_audio.bitrate = audio.bitrate;
}

Logo CTS_OutFile::getLogo()
{
	return _logo;
}
void CTS_OutFile::setLogo(Logo logo)
{
	memcpy(this->_logo.file,logo.file,strlen(logo.file)+1);
	this->_logo.width = logo.width;
	this->_logo.height = logo.height;
	this->_logo.x = logo.x;
	this->_logo.y = logo.y;
	return ;
}
