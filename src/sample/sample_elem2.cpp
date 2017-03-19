#include <tchar.h>
#include "sample_elem2.h"

StkObject* Sample_Elem2::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"Elem2\" : { \"bbb\" : 123, \"ccc\" : \"xyz\" }, \"ddd\" : 987 }"), &ErrorCode);
	*ResultCode = 200;
	return TmpObj;
}
