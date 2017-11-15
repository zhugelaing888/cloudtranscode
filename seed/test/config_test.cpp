//main.cpp
#include "util/cts_config.h"
int main()
{
	std::string http;
	std::string path;
	const char ConfigFile[]= "config.txt"; 
	Config configSettings(ConfigFile);
	
	http = configSettings.Read("http", http);
	path = configSettings.Read("path", path);
	std::cout<<"http:"<<http<<std::endl;
	std::cout<<"path:"<<path<<std::endl;
	
	return 0;
}
