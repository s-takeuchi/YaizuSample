#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostAgentInfo.h"

StkObject* ApiPostAgentInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t* Name = NULL;
	wchar_t* StatusTimeUtc = NULL;
	wchar_t* StatusTimeLocal = NULL;
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* AgentInfo = ReqObj->GetFirstChildElement();
		if (AgentInfo == NULL) {
			return NULL;
		}
		StkObject* AgtInfoName = AgentInfo->GetFirstChildElement();
		if (AgtInfoName == NULL) {
			return NULL;
		}
		StkObject* AgtInfoTimeUtc = AgtInfoName->GetNext();
		if (AgtInfoTimeUtc == NULL) {
			return NULL;
		}
		StkObject* AgtInfoTimeLocal = AgtInfoTimeUtc->GetNext();
		if (AgtInfoTimeLocal == NULL) {
			return NULL;
		}
		Name = AgtInfoName->GetStringValue();
		StatusTimeUtc = AgtInfoTimeUtc->GetStringValue();
		StatusTimeLocal = AgtInfoTimeLocal->GetStringValue();
		DataAccess::GetInstance()->SetAgentInfo(Name, StatusTimeUtc, StatusTimeLocal);
	}

	StkObject* TmpObj = new StkObject(L"");
	*ResultCode = 200;
	return TmpObj;
}
