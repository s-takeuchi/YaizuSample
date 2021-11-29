#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "serval.h"
#include "ApiPostAgentInfo.h"

// Create and generate logs for response
// StatusStr [out] : Acquired string shows status
// Status [in] : Status
void ApiPostAgentInfo::GetStatusStr(wchar_t StatusStr[10], int Status)
{
	switch (Status) {
	case -981:
		StkPlWcsCpy(StatusStr, 10, L"NOCMD");
		break;
	case -990:
		StkPlWcsCpy(StatusStr, 10, L"SFILE");
		break;
	case -991:
		StkPlWcsCpy(StatusStr, 10, L"AFILE");
		break;
	case -992:
		StkPlWcsCpy(StatusStr, 10, L"PLATF");
		break;
	case -993:
		StkPlWcsCpy(StatusStr, 10, L"TIMEO");
		break;
	case -994:
		StkPlWcsCpy(StatusStr, 10, L"AGDIR");
		break;
	case -995:
		StkPlWcsCpy(StatusStr, 10, L"CMRLT");
		break;
	case 0:
		StkPlWcsCpy(StatusStr, 10, L"SUCCS");
		break;
	case 1:
		StkPlWcsCpy(StatusStr, 10, L"FAILD");
		break;
	default:
		StkPlWcsCpy(StatusStr, 10, L"UNKNOWN");
		break;
	}
}


StkObject* ApiPostAgentInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t Name[DA_MAXLEN_OF_AGTNAME] = L"";
	int Status = 0;
	int StatusCmd = -1;
	int OpStatus = 0;
	int OpCmd = -1;
	bool AgentNameFlag = false;
	bool StatusCmdFlag = false;
	bool OpStatusFlag = false;
	bool OpCmdFlag = false;
	long long StatusTime = 0;
	if (ReqObj != NULL) {
		StkObject* CurrAgentInfo = ReqObj->GetFirstChildElement();
		if (CurrAgentInfo == NULL) {
			StkObject* ResObj = new StkObject(L"");
			AddCodeAndMsg(ResObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
			*ResultCode = 400;
			return ResObj;
		}
		while (CurrAgentInfo) {
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"Name") == 0) {
				StkPlWcsCpy(Name, DA_MAXLEN_OF_AGTNAME, CurrAgentInfo->GetStringValue());
				AgentNameFlag = true;
				if (StkPlCheckHostName(Name) == false) {
					*ResultCode = 400;
					StkObject* ResObj = new StkObject(L"");
					CommonError_ForbiddenChar(ResObj, L"Name");
					return ResObj;
				}
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
			if (StkPlWcsCmp(CurrAgentInfo->GetName(), L"AcqTime") == 0) {
				wchar_t StatusTimeStr[DA_MAXLEN_OF_UNIXTIME * 2 + 1];
				StkPlWcsCpy(StatusTimeStr, DA_MAXLEN_OF_UNIXTIME * 2 + 1, CurrAgentInfo->GetStringValue());
				StkPlSwScanf(StatusTimeStr, L"%016x", &StatusTime);
			}
			CurrAgentInfo = CurrAgentInfo->GetNext();
		}
		if (AgentNameFlag == false) {
			*ResultCode = 400;
			StkObject* ResObj = new StkObject(L"");
			CommonError_NoElemInRequest(ResObj, L"Name");
			return ResObj;
		}
		// Check user credential and permission for command change request
		int UserId = -1;
		if (StatusCmdFlag || OpCmdFlag) {
			wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
			if (!CheckCredentials(Token, UserName, &UserId)) {
				StkObject* ResObj = new StkObject(L"");
				AddCodeAndMsg(ResObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
				*ResultCode = 401;
				return ResObj;
			}
			if (!IsAdminUser(Token)) {
				StkObject* ResObj = new StkObject(L"");
				AddCodeAndMsg(ResObj, MSG_NO_EXEC_RIGHT, MessageProc::GetMsgEng(MSG_NO_EXEC_RIGHT), MessageProc::GetMsgJpn(MSG_NO_EXEC_RIGHT));
				*ResultCode = 403;
				return ResObj;
			}
		}
		if (StatusCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForStatusCmd(Name, StatusCmd);

			wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
			DataAccess::GetInstance()->GetCommandNameById(StatusCmd, CmdName);
			wchar_t LogMsg[512] = L"";
			wchar_t LogMsgJa[512] = L"";
			StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDSTATUSACQCHGD), Name, (StatusCmd == -1)? L"(Cleared)" : CmdName);
			StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDSTATUSACQCHGD), Name, (StatusCmd == -1)? L"(Cleared)" : CmdName);
			EventLogging(LogMsg, LogMsgJa, UserId);
		}
		if (OpCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForOpCmd(Name, OpCmd);

			wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
			DataAccess::GetInstance()->GetCommandNameById(OpCmd, CmdName);
			wchar_t LogMsg[512] = L"";
			wchar_t LogMsgJa[512] = L"";
			StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDOPERATIONCHGD), Name, CmdName);
			StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDOPERATIONCHGD), Name, CmdName);
			EventLogging(LogMsg, LogMsgJa, UserId);
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
				EventLogging(LogMsg, LogMsgJa, -1);
			} else {
				DataAccess::GetInstance()->SetAgentInfoForTime(Name, 1);
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				wchar_t OpStatusStr[10] = L"";
				GetStatusStr(OpStatusStr, OpStatus);
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDOPENDED), Name, OpStatusStr);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDOPENDED), Name, OpStatusStr);
				EventLogging(LogMsg, LogMsgJa, -1);
			}
		}
		if (!StatusCmdFlag && !OpCmdFlag && !OpStatusFlag) {
			int RetSetAgtInfo = DataAccess::GetInstance()->SetAgentInfo(Name, Status, StatusTime);
			DataAccess::GetInstance()->AddTimeSeriesData(Name, Status);
			if (RetSetAgtInfo == 0) {
				// Add new agent
				DataAccess::GetInstance()->SetAgentInfoForOpStatus(Name, -984);
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_NEWAGTNOTIFIED), Name);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_NEWAGTNOTIFIED), Name);
				EventLogging(LogMsg, LogMsgJa, -1);
			} else if (RetSetAgtInfo == 1) {
				// Update
			}
			DataAccess::GetInstance()->SetAgentInfoForTime(Name, 0);
		}
	} else {
		*ResultCode = 400;
		StkObject* ResObj = new StkObject(L"");
		AddCodeAndMsg(ResObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
		return ResObj;
	}

	StkObject* TmpObj = new StkObject(L"");
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	*ResultCode = 200;
	return TmpObj;
}
