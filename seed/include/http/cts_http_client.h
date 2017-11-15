#ifndef CTS_HTTP_CLIENT_H__
#define CTS_HTTP_CLIENT_H__

#include <string>

class CTS_CHttpClient
{
public:
	CTS_CHttpClient(void);
	~CTS_CHttpClient(void);

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

