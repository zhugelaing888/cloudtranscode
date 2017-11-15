#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <string>

class CHttpClient
{
public:
	CHttpClient(void);
	~CHttpClient(void);

public:
	int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
	int Get(const std::string & strUrl, std::string & strResponse);

	int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);

public:
	void SetDebug(bool bDebug);

private:
	bool m_bDebug;
};

#endif

