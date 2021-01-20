#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "dataaccess.h"
#include "ApiGetServerInfo.h"

ApiGetServerInfo::ApiGetServerInfo()
{
	StkPlGetWTimeInIso8601(StartTimeUtc, false);
	StkPlGetWTimeInIso8601(StartTimeLocal, true);
}

StkObject* ApiGetServerInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	if (!CheckCredentials(Token, UserName)) {
		*ResultCode = 403;
		return NULL;
	}

	int PInterval = 0;
	int SaInterval = 0;
	DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);
	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjD = new StkObject(L"Data");
	StkObject* TmpObjC = new StkObject(L"ServerInfo");
	TmpObjC->AppendChildElement(new StkObject(L"StartTimeUtc", StartTimeUtc));
	TmpObjC->AppendChildElement(new StkObject(L"StartTimeLocal", StartTimeLocal));
	TmpObjC->AppendChildElement(new StkObject(L"Version", L"1.0.0"));
	TmpObjC->AppendChildElement(new StkObject(L"PollingInterval", PInterval));
	TmpObjC->AppendChildElement(new StkObject(L"StatusAcquisitionInterval", SaInterval));
	TmpObjD->AppendChildElement(TmpObjC);
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
