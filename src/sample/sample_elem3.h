#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class Sample_Elem3 : StkWebAppExec
{
	StkObject* Execute(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
