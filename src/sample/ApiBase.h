﻿#pragma once
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"

class ApiOldBase : public StkWebAppExec
{
protected:
	virtual StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, int, wchar_t*) = 0;
	bool CheckCredentials(wchar_t*, wchar_t* = NULL);
	bool IsAdminUser(wchar_t*);
	StkObject* CommonError_NoElemInRequest(const wchar_t*);
	StkObject* CommonError_StringLenError(const wchar_t*, int);
	StkObject* CommonError_NoRequest();
	StkObject* CommonError_NoExecRight();
	StkObject* CommonError_ForbiddenChar(const wchar_t*);

public:
	virtual StkObject* Execute(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t*);
};
