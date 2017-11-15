#include "http/http_client.h"
#include "json/json.h"
#include <iostream>
#include <unistd.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml; 
using namespace std;
using namespace Json;

#define MAX 10
#define MARK 40
#define SEG_SIZE 50

static string g_filepath("/opt/transcodec/path/xml/");//"/root/ch123/path/xml/"
static string g_httpath("http://192.168.3.145:1234/transcode");

void* httpRequest0(void *info)
{
	int num = 0;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);	
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest1(void *info)
{
	int num = 1;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest2(void *info)
{
	int num = 2;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest3(void *info)
{
	int num = 3;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest4(void *info)
{
	int num = 4;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest5(void *info)
{
	int num = 5;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest6(void *info)
{
	int num = 6;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest7(void *info)
{
	int num = 7;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest8(void *info)
{
	int num = 8;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest9(void *info)
{
	int num = 9;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest10(void *info)
{
	int num = 10;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest11(void *info)
{
	int num = 11;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest12(void *info)
{
	int num = 12;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest13(void *info)
{
	int num = 13;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest14(void *info)
{
	int num = 14;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest15(void *info)
{
	int num = 15;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest16(void *info)
{
	int num = 16;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest17(void *info)
{
	int num = 17;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest18(void *info)
{
	int num = 18;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest19(void *info)
{
	int num = 19;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest20(void *info)
{
	int num = 0;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest21(void *info)
{
	int num = 1;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest22(void *info)
{
	int num = 2;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest23(void *info)
{
	int num = 3;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest24(void *info)
{
	int num = 4;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest25(void *info)
{
	int num = 5;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest26(void *info)
{
	int num = 6;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest27(void *info)
{
	int num = 7;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest28(void *info)
{
	int num = 8;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest29(void *info)
{
	int num = 9;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest30(void *info)
{
	int num = 10;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest31(void *info)
{
	int num = 11;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest32(void *info)
{
	int num = 12;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest33(void *info)
{
	int num = 13;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest34(void *info)
{
	int num = 14;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest35(void *info)
{
	int num = 15;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest36(void *info)
{
	int num = 16;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest37(void *info)
{
	int num = 17;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest38(void *info)
{
	int num = 18;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest39(void *info)
{
	int num = 19;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest40(void *info)
{
	int num = 0;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest41(void *info)
{
	int num = 1;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest42(void *info)
{
	int num = 2;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest43(void *info)
{
	int num = 3;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest44(void *info)
{
	int num = 4;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest45(void *info)
{
	int num = 5;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest46(void *info)
{
	int num = 6;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest47(void *info)
{
	int num = 7;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest48(void *info)
{
	int num = 8;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest49(void *info)
{
	int num = 9;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest50(void *info)
{
	int num = 10;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest51(void *info)
{
	int num = 11;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

void* httpRequest52(void *info)
{
	int num = 12;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest53(void *info)
{
	int num = 13;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest54(void *info)
{
	int num = 14;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest55(void *info)
{
	int num = 15;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest56(void *info)
{
	int num = 16;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest57(void *info)
{
	int num = 17;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest58(void *info)
{
	int num = 18;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}
void* httpRequest59(void *info)
{
	int num = 19;
	CHttpClient client;
	client.SetDebug(true);
	char xml[128];
	for(int i = (MARK+num)*SEG_SIZE;i < (MARK+num+1)*SEG_SIZE;i++)
	{
		sprintf(xml,"%s%d.xml",g_filepath.c_str(),i);
		file<> fdoc(xml); 
		string str = fdoc.data();
		string responseBuf;
		client.Post(g_httpath,str,responseBuf);		
		cout<<responseBuf.c_str()<<endl;
		sleep(1);
	}
	
	return (void*)NULL;
}

int main(int args,char **argv)
{
	pthread_t http_id[MAX];
	pthread_t id;
	int num = 0;
	if(pthread_create(&id,NULL,&httpRequest0,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest1,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest2,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest3,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest4,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest5,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest6,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest7,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest8,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest9,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest10,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest11,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest12,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest13,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest14,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest15,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest16,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest17,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest18,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest19,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	/*
	if(pthread_create(&id,NULL,&httpRequest20,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest21,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest22,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest23,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest24,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest25,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest26,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest27,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest28,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest29,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest30,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest31,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest12,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest33,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest34,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest35,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest36,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest37,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest38,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest39,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	if(pthread_create(&id,NULL,&httpRequest40,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest41,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest42,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest43,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest44,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest45,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest46,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest47,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest48,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest49,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest50,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest51,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest52,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest53,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest54,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest55,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest56,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest57,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest58,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}
	num++;
	if(pthread_create(&id,NULL,&httpRequest59,(void*)&num)!=0)
	{
		printf("create the thread error!");
		return -1;
	}*/
	while(1);
	return 0;
}
 
