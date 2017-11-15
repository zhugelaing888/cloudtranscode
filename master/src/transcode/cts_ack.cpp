#include "transcode/cts_ack.h"
#include "util/util.h"
#include "util/simple_log.h"
#include "http/http_client.h"
#include "json/json.h"

CTS_Ack::CTS_Ack():_id(NULL)
{
}

CTS_Ack::~CTS_Ack()
{
	if(this->_id)
	{
		delete[] this->_id;
		this->_id = NULL;
	}
}

int CTS_Ack::parse(char *id)
{
	if(this->_id == NULL)
		this->_id = new char[strlen(id)+1];
	memcpy(this->_id,id,strlen(id)+1);
	return 0;
}

int CTS_Ack::UpdateAck(const char *httpath,int ecode)
{
	CHttpClient tmp;
	Json::Value arrayItem;
	string response;
	string cmd("content=");
	arrayItem["id"] = this->_id;
	arrayItem["result"] = ecode;
	cmd.append(arrayItem.toStyledString());
	tmp.Post(httpath,cmd,response);
	return tmp.Post(httpath,cmd,response);
}

