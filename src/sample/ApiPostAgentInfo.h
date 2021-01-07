#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiPostAgentInfo : ApiBase
{
private:
	StkObject * CommonError_NoElemInRequest(const wchar_t*);
	StkObject* CommonError_StringLenError(const wchar_t*, int);
	StkObject* CommonError_NoRequest();
	StkObject* CommonError_NoExecRight();
	StkObject* CommonError_ForbiddenChar(const wchar_t*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
