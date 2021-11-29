#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "serval.h"
#include "dataaccess.h"
#include "ApiPostViewSetting.h"

StkObject* ApiPostViewSetting::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}
	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(TmpObj, MSG_NO_EXEC_RIGHT, MessageProc::GetMsgEng(MSG_NO_EXEC_RIGHT), MessageProc::GetMsgJpn(MSG_NO_EXEC_RIGHT));
		*ResultCode = 403;
		return TmpObj;
	}

	if (ReqObj != NULL) {
		StkObject* CurrVsElem = ReqObj->GetFirstChildElement();
		if (CurrVsElem == NULL) {
			*ResultCode = 400;
			AddCodeAndMsg(TmpObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
			return TmpObj;
		}
		while (CurrVsElem) {
			if (StkPlWcsCmp(CurrVsElem->GetName(), L"ViewSetting") == 0) {
				StkObject* CurrVsChild = CurrVsElem->GetFirstChildElement();
				int Index = -1;
				wchar_t TargetAgent[DA_MAXLEN_OF_AGTNAME] = L"";

				// Fetch "Index"
				if (CurrVsChild == NULL || StkPlWcsCmp(CurrVsChild->GetName(), L"Index") != 0) {
					*ResultCode = 400;
					CommonError_NoElemInRequest(TmpObj, L"Index");
					return TmpObj;
				}
				Index = CurrVsChild->GetIntValue();

				// Fetch "TargetAgent"
				CurrVsChild = CurrVsChild->GetNext();
				if (CurrVsChild == NULL || StkPlWcsCmp(CurrVsChild->GetName(), L"TargetAgent") != 0) {
					*ResultCode = 400;
					CommonError_NoElemInRequest(TmpObj, L"TargetAgent");
					return TmpObj;
				}
				StkPlWcsCpy(TargetAgent, DA_MAXLEN_OF_AGTNAME, CurrVsChild->GetStringValue());

				if (Index >= 1 && Index <= 8) {
					wchar_t PropName[256] = L"";
					StkPlSwPrintf(PropName, 256, L"ViewSetting_d%d", Index);
					StkWebAppUm_SetPropertyValueWStr(PropName, TargetAgent);
				}
			}
			CurrVsElem = CurrVsElem->GetNext();
		}
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;
		return TmpObj;
	} else {
		*ResultCode = 400;
		AddCodeAndMsg(TmpObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
		return TmpObj;
	}
}
