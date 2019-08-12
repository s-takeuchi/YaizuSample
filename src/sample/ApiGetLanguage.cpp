#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "ApiGetLanguage.h"

StkObject* ApiGetLanguage::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	if (LocaleType == -1) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", L"Error"));
		*ResultCode = 406;
	} else {
		wchar_t Locale[3];
		if (LocaleType == 0) {
			StkPlWcsCpy(Locale, 3, L"en");
		} else if (LocaleType == 1) {
			StkPlWcsCpy(Locale, 3, L"ja");
		}

		ResObj->AppendChildElement(new StkObject(L"Msg0", L""));
		ResObj->AppendChildElement(new StkObject(L"ClientLanguage", Locale));
		*ResultCode = 200;
	}
	return ResObj;
}
