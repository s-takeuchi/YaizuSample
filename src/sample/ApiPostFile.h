#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiPostFile : ApiBase
{
private:
	const int POSTFILE_TYPE_UPLOAD = 0;
	const int POSTFILE_TYPE_COMMANDRESULT = 1;

private:
	unsigned char* OrgDat[10];
	unsigned char* LockTableOrgDat[10];
	StkPlCriticalSection CsGetFile;

	unsigned char* GetOrgDat();
	void ReleaseOrgDat(unsigned char*);

public:
	ApiPostFile();
	~ApiPostFile();

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
