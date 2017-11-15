/*
 * http_server_test.cpp
 *
 *  Created on: Oct 26, 2014
 *      Author: liao
 */
#include <sstream>
#include <cstdlib>
#include "util/simple_log.h"
#include "http/http_server.h"
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
	if(daemon(1, 0) != 0) 
    {
        printf("Call daemon error" );
        return -1;
    }
    log_init("/root/transcode/httplog/http.log",WARN_LEVEL);
    HttpServer http_server;
   // http_server.add_mapping("/hello", hello);
  //  http_server.add_mapping("/usleep", usleep);
  //  http_server.add_mapping("/sayhello", sayhello);
   // http_server.add_mapping("/login", login, POST_METHOD);
    //http_server.set_schedule(test_schedule);
	http_server.add_mapping("/progress", progress, POST_METHOD);
  	http_server.add_mapping("/result", result, POST_METHOD);
    int port = atoi(args[1]);
    int backlog = 100000;
    int max_events = 1000;
    http_server.start(port, backlog, max_events);
    return 0;
}
