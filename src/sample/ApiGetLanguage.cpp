#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "ApiGetLanguage.h"

StkObject* ApiGetLanguage::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* ResObj = new StkObject(L"");
	if (Locale == NULL || Locale[2] != '\0' || Locale[0] == '\0') {
		ResObj->AppendChildElement(new StkObject(L"Msg0", L"Error"));
		*ResultCode = 406;
	} else {
		ResObj->AppendChildElement(new StkObject(L"Msg0", L""));
		ResObj->AppendChildElement(new StkObject(L"ClientLanguage", Locale));
		*ResultCode = 200;
	}
	return ResObj;
}
