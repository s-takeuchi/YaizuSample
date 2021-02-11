#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiPostServerInfo.h"

StkObject* ApiPostServerInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	if (!CheckCredentials(Token, UserName)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}
	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(TmpObj, MSG_NO_EXEC_RIGHT, MessageProc::GetMsgEng(MSG_NO_EXEC_RIGHT), MessageProc::GetMsgJpn(MSG_NO_EXEC_RIGHT));
		*ResultCode = 403;
		return TmpObj;
	}

	int PInterval = -1;
	int SaInterval = -1;
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* CurObj = ReqObj->GetFirstChildElement();
		if (CurObj == NULL) {
			AddCodeAndMsg(TmpObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
			*ResultCode = 400;
			return TmpObj;
		}
		while (CurObj) {
			if (StkPlWcsCmp(CurObj->GetName(), L"PollingInterval") == 0) {
				PInterval = CurObj->GetIntValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"StatusAcquisitionInterval") == 0) {
				SaInterval = CurObj->GetIntValue();
			}
			CurObj = CurObj->GetNext();
		}
		if (PInterval == -1) {
			AddCodeAndMsg(TmpObj, MSG_NOPOLLINGINTVL, MessageProc::GetMsgEng(MSG_NOPOLLINGINTVL), MessageProc::GetMsgJpn(MSG_NOPOLLINGINTVL));
			*ResultCode = 400;
			return TmpObj;
		}
		if (SaInterval == -1) {
			AddCodeAndMsg(TmpObj, MSG_NOSAINTVL, MessageProc::GetMsgEng(MSG_NOSAINTVL), MessageProc::GetMsgJpn(MSG_NOSAINTVL));
			*ResultCode = 400;
			return TmpObj;
		}
		if (PInterval < 30 || PInterval > 900) {
			AddCodeAndMsg(TmpObj, MSG_INVALIDPOINTVL, MessageProc::GetMsgEng(MSG_INVALIDPOINTVL), MessageProc::GetMsgJpn(MSG_INVALIDPOINTVL));
			*ResultCode = 400;
			return TmpObj;
		}
		if (SaInterval < 300 || SaInterval > 3600) {
			AddCodeAndMsg(TmpObj, MSG_INVALIDSVINTVL, MessageProc::GetMsgEng(MSG_INVALIDSVINTVL), MessageProc::GetMsgJpn(MSG_INVALIDSVINTVL));
			*ResultCode = 400;
			return TmpObj;
		}
		DataAccess::GetInstance()->SetServerInfo(PInterval, SaInterval);

		wchar_t LogMsg[256] = L"";
		wchar_t LogMsgJa[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"%ls [Polling Interval=%d sec, Status Acquisition Interval=%d sec]", MessageProc::GetMsgEng(MSG_SVRINFOUPDATED), PInterval, SaInterval);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [Polling Interval=%d sec, Status Acquisition Interval=%d sec]", MessageProc::GetMsgJpn(MSG_SVRINFOUPDATED), PInterval, SaInterval);
		StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa);
	} else {
		AddCodeAndMsg(TmpObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
		*ResultCode = 400;
		return TmpObj;
	}

	AddCodeAndMsg(TmpObj, 0, L"", L"");
	*ResultCode = 200;
	return TmpObj;
}
