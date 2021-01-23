#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiDeleteCommand.h"

StkObject* ApiDeleteCommand::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TargetIdStr[16] = L"";
	StkStringParser::ParseInto1Param(UrlPath, L"/api/command/$/", L'$', TargetIdStr, 16);
	int TargetId = StkPlWcsToL(TargetIdStr);
	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME];
	int RetName = DataAccess::GetInstance()->GetCommandNameById(TargetId, CmdName);
	int RetDel = DataAccess::GetInstance()->DeleteCommand(TargetId);

	StkObject* TmpObj = new StkObject(L"");
	if (RetName == 0 && RetDel == 0) {
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;

		wchar_t LogMsg[256] = L"";
		wchar_t LogMsgJa[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_COMDELETE), CmdName);
		StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_COMDELETE), CmdName);
		AddLogMsg(LogMsg, LogMsgJa);
	} else {
		AddCodeAndMsg(TmpObj, MSG_COMMANDNOTEXIST, MessageProc::GetMsgEng(MSG_COMMANDNOTEXIST), MessageProc::GetMsgJpn(MSG_COMMANDNOTEXIST));
		*ResultCode = 400;
	}
	return TmpObj;
}
