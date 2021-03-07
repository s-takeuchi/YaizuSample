#pragma once
#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiGetServerInfo : ApiBase
{
private:
	long long StartTime;

public:
	ApiGetServerInfo();
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
