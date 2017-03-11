#include <tchar.h>
#include "sample_elem3.h"

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromXml(_T("<body><h1>Hello, 3</h1></body>"), &ErrorCode);
	*ResultCode = 200;
	return TmpObj;
}
