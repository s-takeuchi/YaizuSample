#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetCommandResult.h"

StkObject* ApiGetCommandResult::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	wchar_t TargetCommandResult[16];

	if (StkStringParser::ParseInto1Param(UrlPath, L"/api/commandresult/$/", L'$', TargetCommandResult, 16) > 0) {
		int ResultId = StkPlWcsToL(TargetCommandResult);
		char Output[DA_MAXLEN_OF_CMDOUTPUT];
		int Length = DataAccess::GetInstance()->GetOutput(ResultId, Output);
		if (Length == -1) {
			AddCodeAndMsg(TmpObj, MSG_RESULT_NOTEXIST, MessageProc::GetMsgEng(MSG_RESULT_NOTEXIST), MessageProc::GetMsgJpn(MSG_RESULT_NOTEXIST));
			*ResultCode = 400;
			return TmpObj;
		}

		AddCodeAndMsg(TmpObj, 0, L"", L"");
		StkObject* TmpObjD = new StkObject(L"Data");
		StkObject* TmpObjC = new StkObject(L"Result");
		wchar_t* TmpBuf = StkPlCreateWideCharFromUtf8(Output);
		size_t TmpBufLen = StkPlWcsLen(TmpBuf);
		wchar_t* TmpOut = new wchar_t[TmpBufLen * 6 + 1];
		TmpOut[0] = L'\0';
		int Index = 0;
		for (int Loop = 0; Loop < TmpBufLen; Loop++) {
			if (TmpBuf[Loop] == L'\n') {
				StkPlWcsCpy(&TmpOut[Index], 6, L"<br/>");
				Index += 5;
				if (Loop < TmpBufLen - 1 && TmpBuf[Loop + 1] == L'\r') {
					Loop++;
				}
			} else if (TmpBuf[Loop] == L'\r') {
				StkPlWcsCpy(&TmpOut[Index], 6, L"<br/>");
				Index += 5;
				if (Loop < TmpBufLen - 1 && TmpBuf[Loop + 1] == L'\n') {
					Loop++;
				}
			} else if (TmpBuf[Loop] == L'<') {
				StkPlWcsCpy(&TmpOut[Index], 5, L"&lt;");
				Index += 4;
			} else if (TmpBuf[Loop] == L'>') {
				StkPlWcsCpy(&TmpOut[Index], 5, L"&gt;");
				Index += 4;
			} else if (TmpBuf[Loop] == L'&') {
				StkPlWcsCpy(&TmpOut[Index], 6, L"&amp;");
				Index += 5;
			} else if (TmpBuf[Loop] == L' ') {
				StkPlWcsCpy(&TmpOut[Index], 7, L"&nbsp;");
				Index += 6;
			} else if (TmpBuf[Loop] == L'\t') {
				StkPlWcsCpy(&TmpOut[Index], 4, L"&#9");
				Index += 3;
			} else {
				TmpOut[Index] = TmpBuf[Loop];
				Index++;
			}
		}
		TmpObjC->AppendChildElement(new StkObject(L"Output", TmpOut));
		delete TmpOut;
		delete TmpBuf;
		TmpObjD->AppendChildElement(TmpObjC);
		TmpObj->AppendChildElement(TmpObjD);
		*ResultCode = 200;
		return TmpObj;
	} else {
		wchar_t AgentName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_AGTNAME];
		wchar_t CommandName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_CMDNAME];
		long long UpdTime[DA_MAXNUM_OF_RESULT];
		int Id[DA_MAXNUM_OF_RESULT];
		int Cnt = DataAccess::GetInstance()->GetCommandResult(AgentName, CommandName, UpdTime, Id);

		AddCodeAndMsg(TmpObj, 0, L"", L"");
		if (Cnt != 0) {
			StkObject* TmpObjD = new StkObject(L"Data");
			for (int Loop = 0; Loop < Cnt; Loop++) {
				StkObject* TmpObjC = new StkObject(L"Result");
				wchar_t UpdTimeStr[DA_MAXLEN_OF_UNIXTIME * 2 + 1] = L"";
				StkPlSwPrintf(UpdTimeStr, DA_MAXLEN_OF_UNIXTIME * 2 + 1, L"%016x", UpdTime[Loop]);

				TmpObjC->AppendChildElement(new StkObject(L"AgentName", AgentName[Loop]));
				TmpObjC->AppendChildElement(new StkObject(L"CommandName", CommandName[Loop]));
				TmpObjC->AppendChildElement(new StkObject(L"UpdTime", UpdTimeStr));
				TmpObjC->AppendChildElement(new StkObject(L"Id", Id[Loop]));
				TmpObjD->AppendChildElement(TmpObjC);
			}
			TmpObj->AppendChildElement(TmpObjD);
		}
		*ResultCode = 200;
		return TmpObj;
	}
}
