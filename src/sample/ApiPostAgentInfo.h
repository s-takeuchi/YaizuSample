#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiPostAgentInfo : ApiBase
{
private:
	void CommonError_NoElemInRequest(StkObject*, const wchar_t*);
	void CommonError_StringLenError(StkObject*, const wchar_t*, int);
	void CommonError_ForbiddenChar(StkObject*, const wchar_t*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
