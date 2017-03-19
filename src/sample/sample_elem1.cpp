#include <tchar.h>
#include "Sample_Elem1.h"

StkObject* Sample_Elem1::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"Elem1\" : { \"bbb\" : 123, \"ccc\" : \"xyz\" }, \"ddd\" : 987 }"), &ErrorCode);
	*ResultCode = 200;
	return TmpObj;
}
