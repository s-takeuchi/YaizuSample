#include <tchar.h>
#include "sample_elem3.h"

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	int ErrCode;
	StkObject* TmpObj;
	if (lstrcmp(Locale, _T("ja")) == 0) {
		TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello2\" : \"‚±‚ñ‚É‚¿‚Í!!\" }"), &ErrCode);
	} else {
		TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello2\" : \"hello, world!!\" }"), &ErrCode);
	}
	*ResultCode = 200;
	return TmpObj;
}
