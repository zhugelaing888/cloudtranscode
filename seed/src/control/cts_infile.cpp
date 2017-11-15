 #include"control/cts_infile.h"
#include "util/cts_log.h"


CTS_InFile::CTS_InFile():_direct(NULL),_path(NULL),_num(0)
{
}

CTS_InFile::~CTS_InFile()
{
	if(this->_direct)
	{
		delete[] this->_direct;
		this->_direct = NULL;
	}
	if(this->_path)
	{
		delete[] this->_path;
		this->_path = NULL;
	}
}

char *CTS_InFile::getDirect()
{
	return _direct;
}
void CTS_InFile::setDirect(char *dir)
{
	if(this->_direct == NULL)
		this->_direct = new char[strlen(dir)+1];
	memcpy(this->_direct,dir,strlen(dir)+1);
}
	
char * CTS_InFile::getPath()
{
	return _path;
}
void CTS_InFile::setPath(char * path)
{
	if(this->_path == NULL)
		this->_path = new char[strlen(path)+1];
	memcpy(_path, path,strlen(path)+1);
	return;
}

int CTS_InFile::getNum()
{
	return _num;
}
void CTS_InFile::setNum(int num)
{
	_num = num;
	return;
}

void CTS_InFile::addSegment(int begin,int end)//
{
	Sgment seg;
	seg.begin = begin;
	seg.end = end;
	
	this->_segment.push_back(seg);
	return;
}

Sgment* CTS_InFile::getSegment()
{
	return NULL;
}
