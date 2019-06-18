#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiPostAgentInfo.h"

StkObject* ApiPostAgentInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t Name[DA_MAXLEN_OF_AGTNAME] = L"";
	int Status = 0;
	int StatusCmd = -1;
	int OpStatus = 0;
	int OpCmd = -1;
	bool StatusCmdFlag = false;
	bool OpStatusFlag = false;
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
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"OpStatus") == 0) {
				OpStatus = CurrAgentInfo->GetIntValue();
				OpStatusFlag = true;
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

			wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
			DataAccess::GetInstance()->GetCommandNameById(StatusCmd, CmdName);
			wchar_t LogMsg[512] = L"";
			wchar_t LogMsgJa[512] = L"";
			StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDSTATUSACQCHGD), Name, CmdName);
			StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDSTATUSACQCHGD), Name, CmdName);
			DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
		}
		if (OpStatusFlag) {
			DataAccess::GetInstance()->SetAgentInfoForOpStatus(Name, OpStatus);

			if (OpStatus == -985) {
				int TmpOpCmd = DataAccess::GetInstance()->GetAgentInfoForOpCmd(Name);
				wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
				DataAccess::GetInstance()->GetCommandNameById(TmpOpCmd, CmdName);
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDOPSTARTED), Name, CmdName);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDOPSTARTED), Name, CmdName);
				DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
			} else {
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %d]", MessageProc::GetMsgEng(MSG_CMDOPENDED), Name, OpStatus);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %d]", MessageProc::GetMsgJpn(MSG_CMDOPENDED), Name, OpStatus);
				DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
			}
		}
		if (OpCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForOpCmd(Name, OpCmd);
		}
		if (!StatusCmdFlag && !OpCmdFlag && !OpStatusFlag) {
			int RetSetAgtInfo = DataAccess::GetInstance()->SetAgentInfo(Name, Status, StatusTimeUtc, StatusTimeLocal);
			if (RetSetAgtInfo == 0) {
				DataAccess::GetInstance()->SetAgentInfoForOpStatus(Name, -984);
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_NEWAGTNOTIFIED), Name);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_NEWAGTNOTIFIED), Name);
				DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
			} else if (RetSetAgtInfo == 1) {
				//
			}
		}
	}

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
