#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiGetServerInfo.h"

ApiGetServerInfo::ApiGetServerInfo()
{
	StartTime = StkPlGetTime();
}

StkObject* ApiGetServerInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	// Build time
	wchar_t BuildDate[32] = L"";
	char BuildDateCh[32] = "";
	StkPlSPrintf(BuildDateCh, 32, "%s  %s", __DATE__, __TIME__);
	StkPlConvUtf8ToWideChar(BuildDate, 32, BuildDateCh);

	// Server start time
	wchar_t StartTimeStr[DA_MAXLEN_OF_UNIXTIME * 2 + 1] = L"";
	StkPlSwPrintf(StartTimeStr, DA_MAXLEN_OF_UNIXTIME * 2 + 1, L"%016x", StartTime);

	// Polling and acquisition interval
	int PInterval = 0;
	int SaInterval = 0;
	DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);

	// Getting used memory of this process
	int PhyMem = StkPlGetUsedMemorySizeOfCurrentProcess();
	int VirMem = StkPlGetUsedVmSizeOfCurrentProcess();

	// Create object to be returned
	StkObject* TmpObjD = new StkObject(L"Data");
	StkObject* TmpObjC = new StkObject(L"ServerInfo");
	TmpObjC->AppendChildElement(new StkObject(L"Version", SERVICE_VERSION_L));
	TmpObjC->AppendChildElement(new StkObject(L"BuildTime", BuildDate));
	TmpObjC->AppendChildElement(new StkObject(L"StartTime", StartTimeStr));
	TmpObjC->AppendChildElement(new StkObject(L"UsedPhysicalMemory", PhyMem));
	TmpObjC->AppendChildElement(new StkObject(L"UsedVirtualMemory", VirMem));
	TmpObjC->AppendChildElement(new StkObject(L"PollingInterval", PInterval));
	TmpObjC->AppendChildElement(new StkObject(L"StatusAcquisitionInterval", SaInterval));
	TmpObjD->AppendChildElement(TmpObjC);
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
