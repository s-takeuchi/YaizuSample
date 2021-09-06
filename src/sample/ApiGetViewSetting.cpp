#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetViewSetting.h"

StkObject* ApiGetViewSetting::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	StkObject* TmpObjD = new StkObject(L"Data");

	for (int Loop = 1; Loop <= 8; Loop++) {
		wchar_t TargetAgent[DA_MAXLEN_OF_AGTNAME] = L"";
		wchar_t PropName[256] = L"";
		StkPlSwPrintf(PropName, 256, L"ViewSetting_d%d", Loop);
		StkWebAppUm_GetPropertyValueWStr(PropName, TargetAgent);

		StkObject* ViewSetting = new StkObject(L"ViewSetting", TargetAgent);
		TmpObjD->AppendChildElement(ViewSetting);
	}
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
