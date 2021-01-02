#pragma once
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "ApiBase.h"

class ApiGetFile : ApiOldBase
{
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, int, wchar_t*);
};
