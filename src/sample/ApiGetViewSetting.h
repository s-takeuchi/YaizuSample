#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/stkobject.h"

class ApiGetViewSetting : ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
