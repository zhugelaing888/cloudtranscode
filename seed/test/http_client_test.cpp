#include "http/cts_http_client.h"
#include "util/cts_log.h"
#include "json/json.h"
#include <iostream>
#include <iostream>
#include <unistd.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml; 
using namespace std;
using namespace Json;
char * getXml(char *filename) 
{ 
	file<> fdoc(filename); 	
	return fdoc.data(); 
}

int main(int args,char **argv)
{
		   log_init("/root/transcode/httplog/http.log",INFO_LEVEL);
		CTS_CHttpClient client;
		client.SetDebug(true);
		string responseBuf;
		string str;
		client.Post("http://192.168.3.145:12345/status",str,responseBuf);
		/*for(int i = 1000;i < 1200;i++)
		{
			sprintf(xml,"/opt/transcodec/src/xml/%d.xml",2);
			file<> fdoc(xml); 
			string str = fdoc.data();
			string responseBuf;
			client.Post("http://192.168.3.145:1234/transcode",str,responseBuf);	
			cout<<responseBuf.c_str()<<endl;
			//client.Post("http://192.168.3.145:1234/transcode",str,responseBuf);	
			//cout<<responseBuf.c_str()<<endl;
			sleep(1);
		}*/
		/*int i = 0;
		char xml[128];
		sprintf(xml,"/opt/transcodec/src/xml/%d.xml",i);
		string str = getXml(xml);
		string responseBuf;
		cout<<client.Post("http://192.168.3.145:1234/transcode",str,responseBuf)<<endl;	
		cout<<responseBuf.c_str()<<endl;
		for(int i = 1;i < 10;i++)
		{
			sprintf(xml,"/opt/transcodec/src/xml/%d.xml",i);
			string str = getXml(xml);
			string responseBuf;
			client.Post("http://192.168.3.145:1234/transcode",str,responseBuf);	
			cout<<responseBuf.c_str()<<endl;
			sleep(1);
		}
		for(int i = 550;i < 560;i++)
		{
			sprintf(xml,"/opt/transcodec/src/xml/%d.xml",i);
			string str = getXml(xml);
			string responseBuf;
			client.Post("http://192.168.3.145:1234/transcode",str,responseBuf);	
			cout<<responseBuf.c_str()<<endl;
			sleep(1);
		}
		for(int i = 650;i < 660;i++)
		{
			sprintf(xml,"/opt/transcodec/src/xml/%d.xml",i);
			string str = getXml(xml);
			string responseBuf;
			client.Post("http://192.168.3.145:1234/transcode",str,responseBuf);	
			cout<<responseBuf.c_str()<<endl;
			sleep(1);
		}*/
        return 0;
}
 
