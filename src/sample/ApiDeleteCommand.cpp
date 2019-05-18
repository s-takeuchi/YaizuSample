#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "dataaccess.h"
#include "ApiDeleteCommand.h"

StkObject* ApiDeleteCommand::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t TargetIdStr[16] = L"";
	StkStringParser::ParseInto1Param(UrlPath, L"/api/command/$/", L'$', TargetIdStr, 16);
	int TargetId = StkPlWcsToL(TargetIdStr);
	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME];
	DataAccess::GetInstance()->GetCommandNameById(TargetId, CmdName);
	DataAccess::GetInstance()->DeleteCommand(TargetId);

	wchar_t LogMsg[256] = L"";
	StkPlSwPrintf(LogMsg, 256, L"A command has been deleted. [%ls]", CmdName);
	DataAccess::GetInstance()->AddLogMsg(LogMsg);

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
