#include <tchar.h>
#include "Sample_Elem1.h"

StkObject* Sample_Elem1::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromXml(_T("<body><h1>Hello, 1</h1></body>"), &ErrorCode);
	*ResultCode = 200;
	return TmpObj;
}
