/*
 * http_server_test.cpp
 *
 *  Created on: Oct 26, 2014
 *      Author: liao
 */
#include <sstream>
#include <cstdlib>
#include "util/cts_log.h"
#include "http/cts_http_server.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void progress(Request &request, Response &response)
{
	Json::Value root;
    std::string progress = request.get_param("content");
	cout<<progress<<endl;
	root["result"]="success";
	root["type"]="progress";
	response.set_body(root);
}

void registerTrans(Request &request, Response &response)
{
	Json::Value root;
    std::string url = request.get_param("url");
	std::string processNum = request.get_param("processNum");
	cout<<"url = "<<url.c_str()<<" processNum = "<<processNum.c_str()<<endl;
	root["result"]="success";
	response.set_body(root);
}

void ackTrans(Request &request, Response &response)
{
	Json::Value root;
    std::string task_id = request.get_param("task_id");
	std::string stat = request.get_param("stat");
	std::string res = request.get_param("result");
	cout<<"task_id = "<<task_id.c_str()<<" stat = "<<stat.c_str()<<" result="<<res.c_str()<<endl;
	root["result"]="success";
	response.set_body(root);
}

void result(Request &request, Response &response)
{
	Json::Value root;
    std::string result = request.get_param("content"); 
	fstream fio("1.txt");
	if( !fio )
	{
	}
	else
	{
		char chArry[128];
    	fio.seekg(0, ios::end);
    	sprintf(chArry,"%s\n",result.c_str());
    	fio.write(chArry, strlen(chArry) );
    	fio.close();
	}
	root["result"]="success";
	root["type"]="result";
	response.set_body(root);
}

void test_schedule() {
    INFO("START schedule ....");
}

int main(int argc, char **args) {
    if (argc < 2) {
        ERROR("usage: ./http_server_test [port]");
        return -1;
    }
	
    log_init("/root/transcode/httplog/http.log",INFO_LEVEL);
    HttpServer http_server;
   // http_server.add_mapping("/hello", hello);
  //  http_server.add_mapping("/usleep", usleep);
  //  http_server.add_mapping("/sayhello", sayhello);
   // http_server.add_mapping("/login", login, POST_METHOD);
    //http_server.set_schedule(test_schedule);
	http_server.add_mapping("/progress", progress, POST_METHOD);
  	http_server.add_mapping("/result", result, POST_METHOD);
	http_server.add_mapping("/registerTrans", registerTrans, POST_METHOD);	
	http_server.add_mapping("/ackTrans",ackTrans,POST_METHOD);
    int port = atoi(args[1]);
    int backlog = 100000;
    int max_events = 1000;
    http_server.start(port, backlog, max_events);
    return 0;
}
