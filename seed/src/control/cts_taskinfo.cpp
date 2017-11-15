 #include "control/cts_taskinfo.h"
#include "util/cts_log.h"

void rapidxml::parse_error_handler(const char *what, void *where)
{
	return ;
}


CTS_TaskInfo::CTS_TaskInfo(std::string cache):_id(NULL),
	_progress(NULL),
	_result(NULL),
	_priority(0),
	ackstat(false),
	delstat(false),
	celstat(false),
	cachefile(cache),
	ossflag(0)
{
	
}
CTS_TaskInfo::CTS_TaskInfo(const char *task_id,const char *result):
	_progress(NULL),
	_priority(0),
	ackstat(false),
	delstat(true),
	celstat(false),
	ossflag(0)
{
	this->_id = new char[strlen(task_id)+1];
	memcpy(this->_id,task_id,strlen(task_id)+1);
	this->_result = new char[strlen(result)+1];
	memcpy(this->_result,result,strlen(result)+1);
	this->_taskack.parse(this->_id);
}

CTS_TaskInfo::~CTS_TaskInfo()
{
	INFO("release the task info!task id :%s",this->_id);
//	while(this->_taskproc.getTheadUpdateStates());//等待进度上传线程结束
	release();
	releaseOutFile();
}

int CTS_TaskInfo::parseAll(rapidxml::xml_document<> *doc)
{
  INFO("begin parse the task info!");
  rapidxml::xml_node<> *root = doc->first_node();   
  rapidxml::xml_node<> *id = root->first_node("id");
  if(!id || id->value_size() == 0)
  {
   	ERROR("the task id is NULL ");
	return -1;
  }
  else
  	this->setId(id->value());
  
  rapidxml::xml_node<> *proc = root->first_node("progress");
  if(!proc || proc->value_size()==0)
  {
   	ERROR("the task progress is NULL");
	return -1;
  }
  else
 	 this->setProgress(proc->value());
  
  rapidxml::xml_node<> *result = root->first_node("result");
  if(!result || result->value_size()==0 )
  {
   	ERROR("the task result is NULL");
	return -1;
  }
  else
 	 this->setResult(result->value()); 
  
  rapidxml::xml_node<> *priority = root->first_node("priority");
  if(!priority || priority->value_size()==0)
  	this->setPriority(0);
  else
 	 this->setPriority(atoi(priority->value()));
  	
  rapidxml::xml_node<> *in = root->first_node("input");
  if(!in) 
  	return -1;
  rapidxml::xml_node<> *infile = in->first_node("file");
  if(infile)
  {  	
		rapidxml::xml_node<> *no = infile->first_node("no");
		if(!no || no->value_size()==0)
			this->_infile.setNum(1);
		else
			this->_infile.setNum(atoi(no->value()));
		rapidxml::xml_node<> *osspath = infile->first_node("tomcat");
		if(!osspath || osspath->value_size()==0)
		{			
		}
		else
		{
			this->ossflag = atoi(osspath->value());
		}
		rapidxml::xml_node<> *path = infile->first_node("path");
		if(!path || path->value_size()==0)
		{
			ERROR("the task infile path is NULL!");
			return -1;
		}
		else
			this->_infile.setPath(path->value());
  }
  else
  	return -1;
  
  rapidxml::xml_node<> *out = root->first_node("output");
  if(!out)
  	return -1;
  string tfile = UTIL::gettimefile();
  for(rapidxml::xml_node<> *oufile = out->first_node("file");oufile;oufile = oufile->next_sibling("file"))
  {
  		string cfile = this->cachefile;
		CTS_OutFile *outfile = new CTS_OutFile;
		if(!oufile)return -1;
		rapidxml::xml_node<> *no = oufile->first_node("no");
		if(no)
		{
			if(no->value_size())
				outfile->setNum(atoi(no->value()));
		}
		rapidxml::xml_node<> *format = oufile->first_node("format");
		if(format)
		{
			if(format->value_size())
				outfile->setFormat(format->value());
		}
		rapidxml::xml_node<> *out_path = oufile->first_node("out_path");
		if(!out_path || out_path->value_size()==0)
		{
			ERROR("the outfile out_path is NULL!");
			return -1;
		}
		else
			outfile->setOutPath(out_path->value());
		rapidxml::xml_node<> *video = oufile->first_node("video");
		if(video)
		{
			Video vid ;
			memset(&vid,0,sizeof(vid));
			rapidxml::xml_node<> *codec = video->first_node("codec");
			rapidxml::xml_node<> *width = video->first_node("width");
			rapidxml::xml_node<> *height = video->first_node("height");
			rapidxml::xml_node<> *lev = video->first_node("profile");
			rapidxml::xml_node<> *bitrate = video->first_node("bitrate");
			rapidxml::xml_node<> *aspect = video->first_node("aspect");

			if(codec)
			{
				if(codec->value_size())
					memcpy(vid.codec,codec->value(),strlen(codec->value())+1);
			}
			if(aspect)
			{
				if(aspect->value_size())
					memcpy(vid.aspect,aspect->value(),strlen(aspect->value())+1);
			}
			if(width)
			{
				if(width->value_size())
					vid.width = atoi(width->value());
			}
			if(height)
			{
				if(height->value_size())
					vid.height = atoi(height->value());
			}
			if(bitrate)
			{
				if(bitrate->value_size())
					vid.bitrate = atoi(bitrate->value());
			}
			if(lev)
			{
				if(lev->value_size())
					vid.lev = profile(atoi(lev->value()));
			}
			outfile->setVideo(vid);
		}
		else
			return -1;
	  	rapidxml::xml_node<> *audio = oufile->first_node("audio");
		if(audio)
		{
	  		Audio aud;
			memset(&aud,0,sizeof(aud));
		  	rapidxml::xml_node<> *codec = audio->first_node("codec");
		  	rapidxml::xml_node<> *sample_rate = audio->first_node("sample_rate");
		  	rapidxml::xml_node<> *channels = audio->first_node("channels");
		  	rapidxml::xml_node<> *bitrate = audio->first_node("bitrate");

			if(codec)
			{
				if(codec->value_size())
					memcpy(aud.codec,codec->value(),strlen(codec->value())+1);
			}
			if(sample_rate)
			{
				if(sample_rate->value_size())
					aud.sample_rate = atoi(sample_rate->value());
			}
			if(channels)
			{
				if(channels->value_size())
					aud.channels = atoi(channels->value());
			}
			if(bitrate)
			{
				if(bitrate->value_size())
					aud.bitrate = atoi(bitrate->value());
			}
		  	outfile->setAudio(aud);
	  	}
		else
			return -1;
  		
	  	rapidxml::xml_node<> *logo = oufile->first_node("logo");
		if(logo)
		{
	  		Logo log;
			memset(&log,0,sizeof(log));
		  	rapidxml::xml_node<> *file = logo->first_node("file");
		  	rapidxml::xml_node<> *width = logo->first_node("width");
		  	rapidxml::xml_node<> *height = logo->first_node("height");
		  	rapidxml::xml_node<> *x = logo->first_node("x");
		  	rapidxml::xml_node<> *y = logo->first_node("y");

			if(file)
			{
				if(file->value_size())
					memcpy(log.file,file->value(),strlen(file->value())+1);
			}
			if(width)
			{
				if(width->value_size())
					log.width = atoi(width->value());
			}
			if(height)
			{
				if(height->value_size())
					log.height = atoi(height->value());
			}
			if(x)
			{
				if(x->value_size())
					log.x = atoi(x->value());
			}
			if(y)
			{
				if(y->value_size())
					log.y = atoi(y->value());
			}
		  	outfile->setLogo(log);
	  	}
		cfile.append("/").append(this->_id).append("_").append(tfile).append("_").append(no->value()).append(".mp4");
		UTIL::CreatDir(cfile.c_str());
		outfile->setCacheFile(cfile);
	  	this->addOutFile(outfile);
  }
  this->_taskproc.parse(this->_id);
  this->_taskack.parse(this->_id); 
  DEBUG("end the task info parse! task id :%s",this->_id);
  return 0;
}
void CTS_TaskInfo::setDuration(int dur)
{
	this->_taskproc.setDuration(dur);
}

void CTS_TaskInfo::setFrames(int frame)
{
	this->_taskproc.setFrames(frame);
}
void CTS_TaskInfo::setAlter(int al)
{
	this->_taskproc.setAlter(al);
}
void CTS_TaskInfo::setThreadUpdateStates()
{
	this->_taskproc.setThreadUpdateStates();
}

int CTS_TaskInfo::UpdateProc()
{
	DEBUG("update progress %s",this->_id);
	int ret = 0;
	if(this->_taskproc.getAlter() == -1)
	{
		DEBUG("update progress alter stat is error!");
		return 1;
	}
	if(this->_progress == NULL)
	{
		WARN("result is NULL!");
		return -1;
	}
	if(this->_taskproc.isAlter())
		ret = this->_taskproc.UpdateProc(this->getProgress());
	return ret; 
}
int CTS_TaskInfo::UpdateAck(const std::string masterServerUrl,int &status,int ecode)
{
	int ret = 0;
	std::string httpath("http://");
	httpath.append(masterServerUrl.c_str()).append("/ackTrans");
	if(this->_result == NULL)
	{
		WARN("result is NULL!");
		return -1;
	}
	status = this->_taskack.UpdateUserAck(this->getResult(),ecode);
	INFO("update seed ack info[%d]! task id :%s,",status,this->_id);
	ret = this->_taskack.UpdateMasterAck(httpath,status,ecode);
	INFO("update master ack info[%d]! task id :%s,",status,this->_id);
	return ret;
}

void CTS_TaskInfo::setCelStatus()
{
	this->celstat = true;
}

bool CTS_TaskInfo::isCelStatus()
{
	return this->celstat;
}

char * CTS_TaskInfo::getId()
{
	return _id;
}
void CTS_TaskInfo::setId(char * id)
{
	if(this->_id == NULL)
		this->_id = new char[strlen(id)+1];
	memcpy(_id,id,strlen(id)+1);
	return ;
}

char * CTS_TaskInfo::getProgress()
{
	return _progress;
}
void CTS_TaskInfo::setProgress(char * pro)
{
	if(this->_progress == NULL)
		this->_progress = new char[strlen(pro)+1];
	memcpy(this->_progress,pro,strlen(pro)+1);
	return;
}

char * CTS_TaskInfo::getResult()
{
	return _result;
}
void CTS_TaskInfo::setResult(char * result)
{
	if(this->_result == NULL)
		this->_result = new char[strlen(result)+1];
	memcpy(this->_result,result,strlen(result)+1);
	return;
}

int CTS_TaskInfo::getPriority()
{
	return _priority;
}
void CTS_TaskInfo::setPriority(int priority)
{
	_priority = priority;
	return;
}

int CTS_TaskInfo::getOutfileSize()
{
	return this->_outfile.size();
}

void CTS_TaskInfo::releaseOutFile()
{
	CTS_OutFile *outfile = NULL;
	list<CTS_OutFile *>::iterator iter;
	for(iter=this->_outfile.begin();iter!=this->_outfile.end();iter++)
	{
		outfile = (*iter);
		iter = this->_outfile.erase(iter);
		if(outfile)
		{
			delete outfile;
			outfile = NULL;
		} 
	}
	return;
}
void CTS_TaskInfo::addOutFile(CTS_OutFile* outfile)
{
	_outfile.push_back(outfile);
	return;
}
void CTS_TaskInfo::release()
{
	if(this->_id)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
	if(this->_result)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
	if(this->_progress)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
	return;
}

