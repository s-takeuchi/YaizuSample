#include <tchar.h>
#include "sample_elem2.h"

StkObject* Sample_Elem2::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	int ErrCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello\" : \"hello, world!!\" }"), &ErrCode);
	*ResultCode = 200;
	return TmpObj;
}
