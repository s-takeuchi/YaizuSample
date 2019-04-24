#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiGetServerInfo.h"

ApiGetServerInfo::ApiGetServerInfo()
{
	StkPlGetWTimeInIso8601(StartTimeUtc, false);
	StkPlGetWTimeInIso8601(StartTimeLocal, true);
}

StkObject* ApiGetServerInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjC = new StkObject(L"ServerInfo");
	TmpObjC->AppendChildElement(new StkObject(L"StartTimeUtc", StartTimeUtc));
	TmpObjC->AppendChildElement(new StkObject(L"StartTimeLocal", StartTimeLocal));
	TmpObjC->AppendChildElement(new StkObject(L"Version", L"1.0.0"));
	TmpObjC->AppendChildElement(new StkObject(L"PollingInterval", 300));
	TmpObjC->AppendChildElement(new StkObject(L"StatusAcquisitionInterval", 1800));
	TmpObj->AppendChildElement(TmpObjC);
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
