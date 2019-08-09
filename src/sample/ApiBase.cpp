#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "ApiBase.h"


StkObject* ApiBase::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t* HttpHeader)
{
	wchar_t Locale[3] = L"";
	int LocaleType = -1;
	StkStringParser::ParseInto2Params(HttpHeader, L"#Accept-Language: #", L'#', NULL, 0, Locale, 3);
	if (Locale != NULL && Locale[2] == '\0' && Locale[0] != '\0' && StkPlWcsCmp(Locale, L"ja") == 0) {
		LocaleType = 1;
		MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_JAPANESE);
	} else if (Locale != NULL && Locale[2] == '\0') {
		LocaleType = 0;
		MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_ENGLISH);
	}

	return ExecuteImpl(ReqObj, Method, UrlPath, ResultCode, LocaleType);
}
