#include "../../../YaizuComLib/src/StkPl.h"
#include "sample_elem3.h"

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	int ErrCode;
	StkObject* TmpObj;
	if (StkPlWcsCmp(Locale, L"ja") == 0) {
		TmpObj = StkObject::CreateObjectFromJson(L"{ \"hello2\" : \"こんにちは!!\" }", &ErrCode);
	} else {
		TmpObj = StkObject::CreateObjectFromJson(L"{ \"hello2\" : \"hello, world!!\" }", &ErrCode);
	}
	*ResultCode = 200;
	return TmpObj;
}
