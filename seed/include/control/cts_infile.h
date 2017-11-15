#ifndef __INFILE_H__
#define __INFILE_H__

#include<iostream>
#include<string.h>
#include<list>
using namespace std;


typedef struct segment
{
	int begin;
	int end;
}Sgment;

class OutFile;

class CTS_InFile
{
public:
	CTS_InFile();
	~CTS_InFile();
	
	char *getDirect();
	void setDirect(char *dir);
	
	char * getPath();
	void setPath(char * path);
	
	int getNum();
	void setNum(int num);
	
	void addSegment(int begin,int end);//
	Sgment *getSegment();
	
private:
		/*�����ļ�����Ŀ¼*/
		char *_direct;
		/* ��ͬ����Э�� */
		char *_path;
		/* ����˳�� */
		int _num;
		list<Sgment> _segment;
};

#endif  //__INFILE_H__
