#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostAgentInfo.h"

StkObject* ApiPostAgentInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t Name[DA_MAXLEN_OF_AGTNAME] = L"";
	int Status = 0;
	int StatusCmd = -1;
	int OpCmd = -1;
	bool StatusCmdFlag = false;
	bool OpCmdFlag = false;
	wchar_t StatusTimeUtc[DA_MAXLEN_OF_TIME] = L"";
	wchar_t StatusTimeLocal[DA_MAXLEN_OF_TIME] = L"";
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
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"StatusCmd") == 0) {
				StatusCmd = CurrAgentInfo->GetIntValue();
				StatusCmdFlag = true;
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"OpCmd") == 0) {
				OpCmd = CurrAgentInfo->GetIntValue();
				OpCmdFlag = true;
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"StatusTimeUtc") == 0) {
				StkPlWcsCpy(StatusTimeUtc, DA_MAXLEN_OF_TIME, CurrAgentInfo->GetStringValue());
			}
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"StatusTimeLocal") == 0) {
				StkPlWcsCpy(StatusTimeLocal, DA_MAXLEN_OF_TIME, CurrAgentInfo->GetStringValue());
			}
			CurrAgentInfo = CurrAgentInfo->GetNext();
		}
		if (StatusCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForStatusCmd(Name, StatusCmd);
		}
		if (OpCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForOpCmd(Name, OpCmd);
		}
		if (!StatusCmdFlag && !OpCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfo(Name, Status, StatusTimeUtc, StatusTimeLocal);
		}
	}

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
