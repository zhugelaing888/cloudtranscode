#include "util/licence.h"
#include "util/util.h"
using namespace LICENCE;
string g_infile("./conf/transcode");
int main()
{
	LICENCE::init();
	///cout<<"33333: "<<LICENCE::checkTime()<<endl;
	cout<<LICENCE::getPthreadNum()<<endl;
	LICENCE::destroy();
	//int no = UTIL::DaysBetween2Days("2015-12-4","2016-11-24");
	//cout<<no<<endl;
	return 0;
}