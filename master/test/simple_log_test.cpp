
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include "util/simple_log.h"

int main(int argc, char **argv) 
{
	char filename[] = "log/test.log";
  log_init(filename,DEBUG_LEVEL);
 
	struct timeval start, end;
	gettimeofday(&start, NULL);
	while(1)
	{
		INFO("%s", "this is a info log");
		DEBUG("%s", "this is a DEBUG log");
		PRINT("%s","this is a PRINT log");
		ERROR("%s", "this is a DEBUG log");
		WARN("%s", "this is a DEBUG log");
		sleep(1);
	}

	while(1);
	gettimeofday(&end, NULL);
	int cost_time = (end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec)/1000;
	INFO("RUN TIME: 4, cost_time:%d ms", cost_time);
	return 0;
}
