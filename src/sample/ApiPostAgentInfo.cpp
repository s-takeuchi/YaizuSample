#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostAgentInfo.h"

StkObject* ApiPostAgentInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t Name[DA_MAXLEN_OF_AGTNAME];
	int Status = 0;
	wchar_t StatusTimeUtc[DA_MAXLEN_OF_TIME];
	wchar_t StatusTimeLocal[DA_MAXLEN_OF_TIME];
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* AgentInfo = ReqObj->GetFirstChildElement();
		if (AgentInfo == NULL) {
			return NULL;
		}
		StkObject* CurrAgentInfo = AgentInfo->GetFirstChildElement();
		while (CurrAgentInfo) {
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"Name") == 0) {
				StkPlWcsCpy(Name, DA_MAXLEN_OF_AGTNAME, CurrAgentInfo->GetStringValue());
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"Status") == 0) {
				Status = CurrAgentInfo->GetIntValue();
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"StatusTimeUtc") == 0) {
				StkPlWcsCpy(StatusTimeUtc, DA_MAXLEN_OF_TIME, CurrAgentInfo->GetStringValue());
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"StatusTimeLocal") == 0) {
				StkPlWcsCpy(StatusTimeLocal, DA_MAXLEN_OF_TIME, CurrAgentInfo->GetStringValue());
			}
			CurrAgentInfo = CurrAgentInfo->GetNext();
		}
		DataAccess::GetInstance()->SetAgentInfo(Name, Status, StatusTimeUtc, StatusTimeLocal);
	}

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
