#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "ApiBase.h"
#include "dataaccess.h"

bool ApiBase::CheckCredentials(wchar_t* Token, wchar_t* Name)
{
	if (Token == NULL || *Token == L'\0') {
		return false;
	}

	wchar_t TmpName[DA_MAXLEN_OF_USERNAME] = L"";
	wchar_t TmpPassword[DA_MAXLEN_OF_PASSWORD] = L"";
	StkStringParser::ParseInto2Params(Token, L"# #", L'#', TmpName, 256, TmpPassword, 32);
	if (TmpName == NULL || TmpPassword == NULL || *TmpName == L'\0' || *TmpPassword == L'\0') {
		return false;
	}

	int Id = 0;
	wchar_t Password[DA_MAXLEN_OF_PASSWORD];
	int Role = 0;
	wchar_t Url[DA_MAXLEN_OF_TARGETURL];
	bool Ret = DataAccess::GetInstance()->GetTargetUserByName(TmpName, &Id, Password, &Role, Url);
	if (Ret == false) {
		return false;
	}

	if (StkPlWcsCmp(TmpPassword, Password) == 0) {
		if (Name != NULL) {
			StkPlWcsCpy(Name, DA_MAXLEN_OF_USERNAME, TmpName);
		}
		return true;
	} else {
		return false;
	}
}

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

	wchar_t Token[256] = L"";
	StkStringParser::ParseInto4Params(HttpHeader, L"#Authorization:#Bearer #\r\n#", L'#', NULL, 0, NULL, 0, Token, 256, NULL, 0);

	return ExecuteImpl(ReqObj, Method, UrlPath, ResultCode, LocaleType, Token);
}
