#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostServerInfo.h"

StkObject* ApiPostServerInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	int PInterval = 0;
	int SaInterval = 0;
	wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH] = L"";
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* ServerInfo = ReqObj->GetFirstChildElement();
		if (ServerInfo == NULL) {
			return NULL;
		}
		StkObject* CurObj = ServerInfo->GetFirstChildElement();
		while (CurObj) {
			if (StkPlWcsCmp(CurObj->GetName(), L"PollingInterval") == 0) {
				PInterval = CurObj->GetIntValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"StatusAcquisitionInterval") == 0) {
				SaInterval = CurObj->GetIntValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"BucketPath") == 0) {
				StkPlWcsCpy(BucketPath, DA_MAXLEN_OF_BUCKETPATH, CurObj->GetStringValue());
			}
			CurObj = CurObj->GetNext();
		}
		StkObject* PIntervalObj = ServerInfo->GetFirstChildElement();
		if (PIntervalObj == NULL) {
			return NULL;
		}
		StkObject* SaIntervalObj = PIntervalObj->GetNext();
		if (SaIntervalObj == NULL) {
			return NULL;
		}
		PInterval = PIntervalObj->GetIntValue();
		SaInterval = SaIntervalObj->GetIntValue();
		DataAccess::GetInstance()->SetServerInfo(PInterval, SaInterval, BucketPath);

		wchar_t LogMsg[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"Server information has been changed. [Polling Interval=%d sec, Status Acquisition Interval=%d sec, Bucket Path=%ls]", PInterval, SaInterval, BucketPath);
		DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsg);
	}

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
