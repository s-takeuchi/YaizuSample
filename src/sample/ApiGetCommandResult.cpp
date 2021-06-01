#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetCommandResult.h"

StkObject* ApiGetCommandResult::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t AgentName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_AGTNAME];
	wchar_t CommandName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_CMDNAME];
	long long UpdTime[DA_MAXNUM_OF_RESULT];
	int Cnt = DataAccess::GetInstance()->GetCommandResult(AgentName, CommandName, UpdTime);

	StkObject* TmpObj = new StkObject(L"");
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
			TmpObjD->AppendChildElement(TmpObjC);
		}
		TmpObj->AppendChildElement(TmpObjD);
	}
	*ResultCode = 200;
	return TmpObj;
}
