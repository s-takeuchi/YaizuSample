#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiPostFile : ApiBase
{
private:
	const int POSTFILE_TYPE_UPLOAD = 0;
	const int POSTFILE_TYPE_COMMANDRESULT = 1;

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
