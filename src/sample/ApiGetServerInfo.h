#pragma once
#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiGetServerInfo : ApiBase
{
private:
	wchar_t StartTimeUtc[DA_MAXLEN_OF_TIME];
	wchar_t StartTimeLocal[DA_MAXLEN_OF_TIME];

public:
	ApiGetServerInfo();
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
