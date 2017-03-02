#include <tchar.h>
#include "sample_elem3.h"

BOOL Sample_Elem3::TermFlag;

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	int ErrorCode;
	StkObject* TmpObj = StkObject::CreateObjectFromXml(_T("<Accepted/>"), &ErrorCode);
	*ResultCode = 200;
	Sample_Elem3::TermFlag = TRUE;
	return TmpObj;
}
