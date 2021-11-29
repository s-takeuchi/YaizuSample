#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "dataaccess.h"
#include "serval.h"
#include "ApiDeleteCommand.h"

StkObject* ApiDeleteCommand::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}
	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(TmpObj, MSG_NO_EXEC_RIGHT, MessageProc::GetMsgEng(MSG_NO_EXEC_RIGHT), MessageProc::GetMsgJpn(MSG_NO_EXEC_RIGHT));
		*ResultCode = 403;
		return TmpObj;
	}

	wchar_t TargetIdStr[16] = L"";
	StkStringParser::ParseInto1Param(UrlPath, L"/api/command/$/", L'$', TargetIdStr, 16);
	int TargetId = StkPlWcsToL(TargetIdStr);
	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME];
	int RetName = DataAccess::GetInstance()->GetCommandNameById(TargetId, CmdName);
	int RetDel = DataAccess::GetInstance()->DeleteCommand(TargetId);

	if (RetName == 0 && RetDel == 0) {
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;

		wchar_t LogMsg[256] = L"";
		wchar_t LogMsgJa[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_COMDELETE), CmdName);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_COMDELETE), CmdName);
		EventLogging(LogMsg, LogMsgJa, UserId);
	} else {
		AddCodeAndMsg(TmpObj, MSG_COMMANDNOTEXIST, MessageProc::GetMsgEng(MSG_COMMANDNOTEXIST), MessageProc::GetMsgJpn(MSG_COMMANDNOTEXIST));
		*ResultCode = 400;
	}
	return TmpObj;
}
