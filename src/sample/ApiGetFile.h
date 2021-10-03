#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiGetFile : ApiBase
{
private:
	char* OrgDat[10];
	wchar_t* HexDat[10];
	char* LockTableOrgDat[10];
	wchar_t* LockTableHexDat[10];
	StkPlCriticalSection CsGetFile;

	char* GetOrgDat();
	void ReleaseOrgDat(char*);
	wchar_t* GetHexDat();
	void ReleaseHexDat(wchar_t*);

public:
	ApiGetFile();
	~ApiGetFile();

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
