#include "sample_elem2.h"

StkObject* Sample_Elem2::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	int ErrCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(L"{ \"hello\" : \"hello, world!!\" }", &ErrCode);
	*ResultCode = 200;
	return TmpObj;
}
