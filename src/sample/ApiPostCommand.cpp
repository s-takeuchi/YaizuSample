#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiPostCommand.h"

bool ApiPostCommand::CheckFileNameChar(wchar_t* Ptr)
{
	while (*Ptr != L'\0') {
		if (*Ptr == L'\\' || *Ptr == L'/' || *Ptr == L':' || *Ptr == L'*' || *Ptr == L'?' || *Ptr == L'\"' || *Ptr == L'<' || *Ptr == L'>' || *Ptr == L'|' || *Ptr == L'&' || *Ptr == L'%' || *Ptr == L' ') {
			return false;
		}
		Ptr++;
	}
	return true;
}

StkObject* ApiPostCommand::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	if (ReqObj == NULL) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOREQUEST)));
		*ResultCode = 400;
		return ResObj;
	}
	StkObject* CommandObj = ReqObj->GetFirstChildElement();
	if (CommandObj == NULL) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOCOMMANDREQUEST)));
		*ResultCode = 400;
		return ResObj;
	}
	int Id = -1;
	wchar_t Name[DA_MAXLEN_OF_CMDNAME] = L"";
	int Type = 0;
	wchar_t Script[DA_MAXLEN_OF_CMDSCRIPT] = L"";
	wchar_t ServerFileName[DA_MAXLEN_OF_SERVERFILENAME] = L"";
	wchar_t AgentFileName[DA_MAXLEN_OF_SERVERFILENAME] = L"";
	StkObject* CurObj = CommandObj->GetFirstChildElement();
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Id") == 0) {
			Id = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Name") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_CMDNAME) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_COMNAMELENGTHERR)));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Type") == 0) {
			Type = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Script") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_CMDSCRIPT / 2) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_COMSCRIPTLENERR)));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Script, DA_MAXLEN_OF_CMDSCRIPT, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"ServerFileName") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_SERVERFILENAME) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_FILENAMELENERR)));
				*ResultCode = 400;
				return ResObj;
			}
			if (!CheckFileNameChar(CurObj->GetStringValue())) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_FILENAMEFBDNCHAR)));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(ServerFileName, DA_MAXLEN_OF_SERVERFILENAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"AgentFileName") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_AGENTFILENAME) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_FILENAMELENERR)));
				*ResultCode = 400;
				return ResObj;
			}
			if (!CheckFileNameChar(CurObj->GetStringValue())) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_FILENAMEFBDNCHAR)));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(AgentFileName, DA_MAXLEN_OF_AGENTFILENAME, CurObj->GetStringValue());
		}
		CurObj = CurObj->GetNext();
	}
	if (Id == -1) {
		// for command addition
		if (DataAccess::GetInstance()->CheckCommandExistenceByName(Name)) {
			ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_DUPCMDNAME)));
			*ResultCode = 400;
			return ResObj;
		}
		Id = DataAccess::GetInstance()->GetMaxCommandId();
		Id++;
		DataAccess::GetInstance()->SetMaxCommandId(Id);
	} else {
		// for command updation
		wchar_t TmpName[DA_MAXLEN_OF_CMDNAME];
		DataAccess::GetInstance()->GetCommandNameById(Id, TmpName);
		if (StkPlWcsCmp(TmpName, Name) != 0) {
			if (DataAccess::GetInstance()->CheckCommandExistenceByName(Name)) {
				ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_DUPCMDNAME)));
				*ResultCode = 400;
				return ResObj;
			}
		}
		if (DataAccess::GetInstance()->GetCommandNameById(Id, TmpName) != 0) {
			ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_COMMANDNOTEXIST)));
			*ResultCode = 400;
			return ResObj;
		}
	}
	if (StkPlWcsCmp(Name, L"") == 0) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsg(MSG_NOCOMMANDNAME)));
		*ResultCode = 400;
		return ResObj;
	}
	int RetSetCom = DataAccess::GetInstance()->SetCommand(Id, Name, Type, (char*)Script, ServerFileName, AgentFileName);
	wchar_t LogMsg[256] = L"";
	wchar_t LogMsgJa[256] = L"";
	if (RetSetCom == 0) {
		StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_COMADD), Name);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_COMADD), Name);
	} else if (RetSetCom == 1) {
		StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_COMMODIFY), Name);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_COMMODIFY), Name);
	}
	AddLogMsg(LogMsg, LogMsgJa);

	ResObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return ResObj;
}
