#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiPostServerInfo.h"

StkObject* ApiPostServerInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType)
{
	int PInterval = 0;
	int SaInterval = 0;
	wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH] = L"";
	StkObject* TmpObj = new StkObject(L"");
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* ServerInfo = ReqObj->GetFirstChildElement();
		if (ServerInfo == NULL) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOSVRINFOREQUEST)));
			*ResultCode = 400;
			return TmpObj;
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
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOPOLLINGINTVL)));
			*ResultCode = 400;
			return TmpObj;
		}
		StkObject* SaIntervalObj = PIntervalObj->GetNext();
		if (SaIntervalObj == NULL) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOSAINTVL)));
			*ResultCode = 400;
			return TmpObj;
		}
		PInterval = PIntervalObj->GetIntValue();
		SaInterval = SaIntervalObj->GetIntValue();
		if (PInterval < 30 || PInterval > 900) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_INVALIDPOINTVL)));
			*ResultCode = 400;
			return TmpObj;
		}
		if (SaInterval < 300 || SaInterval > 3600) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_INVALIDSVINTVL)));
			*ResultCode = 400;
			return TmpObj;
		}
		DataAccess::GetInstance()->SetServerInfo(PInterval, SaInterval, BucketPath);

		wchar_t LogMsg[256] = L"";
		wchar_t LogMsgJa[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"%ls [Polling Interval=%d sec, Status Acquisition Interval=%d sec, Bucket Path=%ls]", MessageProc::GetMsgEng(MSG_SVRINFOUPDATED), PInterval, SaInterval, BucketPath);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [Polling Interval=%d sec, Status Acquisition Interval=%d sec, Bucket Path=%ls]", MessageProc::GetMsgJpn(MSG_SVRINFOUPDATED), PInterval, SaInterval, BucketPath);
		DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
	} else {
		TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOREQUEST)));
		*ResultCode = 400;
		return TmpObj;
	}

	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
