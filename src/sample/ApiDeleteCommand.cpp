#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiDeleteCommand.h"

StkObject* ApiDeleteCommand::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t TargetIdStr[16] = L"";
	StkStringParser::ParseInto1Param(UrlPath, L"/api/command/$/", L'$', TargetIdStr, 16);
	int TargetId = StkPlWcsToL(TargetIdStr);
	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME];
	int RetName = DataAccess::GetInstance()->GetCommandNameById(TargetId, CmdName);
	int RetDel = DataAccess::GetInstance()->DeleteCommand(TargetId);

	StkObject* TmpObj = new StkObject(L"");
	if (RetName == 0 && RetDel == 0) {
		TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
		*ResultCode = 200;

		wchar_t LogMsg[256] = L"";
		wchar_t LogMsgJa[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_COMDELETE), CmdName);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_COMDELETE), CmdName);
		DataAccess::GetInstance()->AddLogMsg(LogMsg, LogMsgJa);
	} else {
		if (Locale != NULL && Locale[2] == '\0' && Locale[0] != '\0' && StkPlWcsCmp(Locale, L"ja") == 0) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsgJpn(MSG_COMMANDNOTEXIST)));
		} else {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", MessageProc::GetMsgEng(MSG_COMMANDNOTEXIST)));
		}
		*ResultCode = 400;
	}
	return TmpObj;
}
