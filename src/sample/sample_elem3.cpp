#include <tchar.h>
#include "sample_elem3.h"

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello2\" : \"hello, world2!!\" }"), &ErrCode);
	*ResultCode = 200;
	return TmpObj;
}
