#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
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

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(ResObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}
	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(ResObj, MSG_NO_EXEC_RIGHT, MessageProc::GetMsgEng(MSG_NO_EXEC_RIGHT), MessageProc::GetMsgJpn(MSG_NO_EXEC_RIGHT));
		*ResultCode = 403;
		return ResObj;
	}

	if (ReqObj == NULL) {
		AddCodeAndMsg(ResObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
		*ResultCode = 400;
		return ResObj;
	}
	StkObject* CurObj = ReqObj->GetFirstChildElement();
	if (CurObj == NULL) {
		AddCodeAndMsg(ResObj, MSG_NOREQUEST, MessageProc::GetMsgEng(MSG_NOREQUEST), MessageProc::GetMsgJpn(MSG_NOREQUEST));
		*ResultCode = 400;
		return ResObj;
	}
	int Id = -1;
	wchar_t Name[DA_MAXLEN_OF_CMDNAME] = L"";
	int Type = 0;
	wchar_t Script[DA_MAXLEN_OF_CMDSCRIPT] = L"";
	wchar_t ServerFileName[5][DA_MAXLEN_OF_SERVERFILENAME] = { L"", L"", L"", L"", L"" };
	wchar_t AgentFileName[5][DA_MAXLEN_OF_SERVERFILENAME] = { L"", L"", L"", L"", L"" };
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Id") == 0) {
			Id = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Name") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_CMDNAME) {
				AddCodeAndMsg(ResObj, MSG_COMNAMELENGTHERR, MessageProc::GetMsgEng(MSG_COMNAMELENGTHERR), MessageProc::GetMsgJpn(MSG_COMNAMELENGTHERR));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Type") == 0) {
			Type = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Script") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_CMDSCRIPT / 2) {
				AddCodeAndMsg(ResObj, MSG_COMSCRIPTLENERR, MessageProc::GetMsgEng(MSG_COMSCRIPTLENERR), MessageProc::GetMsgJpn(MSG_COMSCRIPTLENERR));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Script, DA_MAXLEN_OF_CMDSCRIPT, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"ServerFileName") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_SERVERFILENAME) {
				AddCodeAndMsg(ResObj, MSG_FILENAMELENERR, MessageProc::GetMsgEng(MSG_FILENAMELENERR), MessageProc::GetMsgJpn(MSG_FILENAMELENERR));
				*ResultCode = 400;
				return ResObj;
			}
			if (!CheckFileNameChar(CurObj->GetStringValue())) {
				AddCodeAndMsg(ResObj, MSG_FILENAMEFBDNCHAR, MessageProc::GetMsgEng(MSG_FILENAMEFBDNCHAR), MessageProc::GetMsgJpn(MSG_FILENAMEFBDNCHAR));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(ServerFileName[0], DA_MAXLEN_OF_SERVERFILENAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"AgentFileName") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_AGENTFILENAME) {
				AddCodeAndMsg(ResObj, MSG_FILENAMELENERR, MessageProc::GetMsgEng(MSG_FILENAMELENERR), MessageProc::GetMsgJpn(MSG_FILENAMELENERR));
				*ResultCode = 400;
				return ResObj;
			}
			if (!CheckFileNameChar(CurObj->GetStringValue())) {
				AddCodeAndMsg(ResObj, MSG_FILENAMEFBDNCHAR, MessageProc::GetMsgEng(MSG_FILENAMEFBDNCHAR), MessageProc::GetMsgJpn(MSG_FILENAMEFBDNCHAR));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(AgentFileName[0], DA_MAXLEN_OF_AGENTFILENAME, CurObj->GetStringValue());
		}
		CurObj = CurObj->GetNext();
	}
	if (Id == -1) {
		// for command addition
		if (DataAccess::GetInstance()->CheckCommandExistenceByName(Name)) {
			AddCodeAndMsg(ResObj, MSG_DUPCMDNAME, MessageProc::GetMsgEng(MSG_DUPCMDNAME), MessageProc::GetMsgJpn(MSG_DUPCMDNAME));
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
				AddCodeAndMsg(ResObj, MSG_DUPCMDNAME, MessageProc::GetMsgEng(MSG_DUPCMDNAME), MessageProc::GetMsgJpn(MSG_DUPCMDNAME));
				*ResultCode = 400;
				return ResObj;
			}
		}
		if (DataAccess::GetInstance()->GetCommandNameById(Id, TmpName) != 0) {
			AddCodeAndMsg(ResObj, MSG_COMMANDNOTEXIST, MessageProc::GetMsgEng(MSG_COMMANDNOTEXIST), MessageProc::GetMsgJpn(MSG_COMMANDNOTEXIST));
			*ResultCode = 400;
			return ResObj;
		}
	}
	if (StkPlWcsCmp(Name, L"") == 0) {
		AddCodeAndMsg(ResObj, MSG_NOCOMMANDNAME, MessageProc::GetMsgEng(MSG_NOCOMMANDNAME), MessageProc::GetMsgJpn(MSG_NOCOMMANDNAME));
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
	EventLogging(LogMsg, LogMsgJa, UserId);

	AddCodeAndMsg(ResObj, 0, L"", L"");
	*ResultCode = 200;
	return ResObj;
}
