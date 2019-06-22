#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiPostCommand.h"

StkObject* ApiPostCommand::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	if (Locale != NULL && Locale[2] == '\0' && Locale[0] != '\0' && StkPlWcsCmp(Locale, L"ja") == 0) {
		MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_JAPANESE);
	} else {
		MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_ENGLISH);
	}
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
			StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Type") == 0) {
			Type = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Script") == 0) {
			StkPlWcsCpy(Script, DA_MAXLEN_OF_CMDSCRIPT, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"ServerFileName") == 0) {
			StkPlWcsCpy(ServerFileName, DA_MAXLEN_OF_SERVERFILENAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"AgentFileName") == 0) {
			StkPlWcsCpy(AgentFileName, DA_MAXLEN_OF_AGENTFILENAME, CurObj->GetStringValue());
		}
		CurObj = CurObj->GetNext();
	}
	if (Id == -1) {
		Id = DataAccess::GetInstance()->GetMaxCommandId();
		Id++;
		DataAccess::GetInstance()->SetMaxCommandId(Id);
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
	DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);

	ResObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return ResObj;
}
