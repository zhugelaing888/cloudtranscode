#include "http/http_client.h"
#include "json/json.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

using namespace std;
using namespace Json;

//argv[1] = ip; argv[2] = task_id;
int main(int args,char **argv) 
{
        CHttpClient tmp;
        string response;
        char httpath[128];
        if(args<3)
        {
        	cout<<"args error"<<endl;
        	return -1;
        }
        memset(httpath,0,sizeof(httpath));
        sprintf(httpath,"http://%s/cancelTrans",argv[1]);
                
        string cmd("task_id=");
        cmd.append(argv[2]);
        return tmp.Post(httpath,cmd,response);   
        cout<<response.c_str()<<endl;
        return 0;
}
 
