#pragma once
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"

class ApiBase : public StkWebAppExec
{
protected:
	virtual StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, int, wchar_t*) = 0;
	bool CheckCredentials(wchar_t*);
public:
	virtual StkObject* Execute(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t*);
};
