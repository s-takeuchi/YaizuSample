#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "sample.h"
#include "ApiDeleteFile.h"

StkObject* ApiDeleteFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
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

	wchar_t FileNameStr[FILENAME_MAX] = L"";
	StkStringParser::ParseInto1Param(UrlPath, L"/api/file/$/", L'$', FileNameStr, FILENAME_MAX);

	wchar_t TargetFullPath[FILENAME_MAX];
	GetFullPathFromFileName(TargetFullPath, FileNameStr);
	StkPlDeleteFile(TargetFullPath);
	AddCodeAndMsg(TmpObj, 0, L"", L"");

	return TmpObj;
}
