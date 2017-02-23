#include <tchar.h>
#include "sample_elem2.h"

StkObject* Sample_Elem2::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromXml(_T("<body><h1>Hello, 2</h1></body>"), &ErrorCode);
	*ResultCode = 200;
	return TmpObj;
}
