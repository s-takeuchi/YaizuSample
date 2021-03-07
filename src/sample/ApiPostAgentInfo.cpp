#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiPostAgentInfo.h"

void ApiPostAgentInfo::CommonError_NoElemInRequest(StkObject* TmpObj, const wchar_t* ElemName)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_NO_ELEM_IN_REQUEST), ElemName);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_NO_ELEM_IN_REQUEST), ElemName);
	AddCodeAndMsg(TmpObj, MSG_NO_ELEM_IN_REQUEST, MsgEng, MsgJpn);
}

void ApiPostAgentInfo::CommonError_StringLenError(StkObject* TmpObj, const wchar_t* Name, int MaxLen)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_STRING_LEN_ERROR), Name, MaxLen);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_STRING_LEN_ERROR), Name, MaxLen);
	AddCodeAndMsg(TmpObj, MSG_STRING_LEN_ERROR, MsgEng, MsgJpn);
}

void ApiPostAgentInfo::CommonError_ForbiddenChar(StkObject* TmpObj, const wchar_t* Name)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_FORBIDDEN_CHAR), Name);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_FORBIDDEN_CHAR), Name);
	AddCodeAndMsg(TmpObj, MSG_FORBIDDEN_CHAR, MsgEng, MsgJpn);
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
		if (StatusCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForStatusCmd(Name, StatusCmd);

			wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
			DataAccess::GetInstance()->GetCommandNameById(StatusCmd, CmdName);
			wchar_t LogMsg[512] = L"";
			wchar_t LogMsgJa[512] = L"";
			StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgEng(MSG_CMDSTATUSACQCHGD), Name, CmdName);
			StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %ls]", MessageProc::GetMsgJpn(MSG_CMDSTATUSACQCHGD), Name, CmdName);
			StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa);
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
				StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa);
			} else {
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls, %d]", MessageProc::GetMsgEng(MSG_CMDOPENDED), Name, OpStatus);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls, %d]", MessageProc::GetMsgJpn(MSG_CMDOPENDED), Name, OpStatus);
				StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa);
			}
		}
		if (OpCmdFlag) {
			DataAccess::GetInstance()->SetAgentInfoForOpCmd(Name, OpCmd);
		}
		if (!StatusCmdFlag && !OpCmdFlag && !OpStatusFlag) {
			int RetSetAgtInfo = DataAccess::GetInstance()->SetAgentInfo(Name, Status, StatusTime);
			if (RetSetAgtInfo == 0) {
				// Addition
				DataAccess::GetInstance()->SetAgentInfoForOpStatus(Name, -984);
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_NEWAGTNOTIFIED), Name);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_NEWAGTNOTIFIED), Name);
				StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa);
			} else if (RetSetAgtInfo == 1) {
				// Update
			}
			DataAccess::GetInstance()->SetAgentInfoForReqTime(Name);
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
