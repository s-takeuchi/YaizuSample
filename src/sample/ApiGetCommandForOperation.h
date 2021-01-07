#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiGetCommandForOperation : ApiBase
{
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
public:
		static bool StopFlag;
};
