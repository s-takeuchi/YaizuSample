#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetTimeSeriesData.h"

StkObject* ApiGetTimeSeriesData::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	wchar_t TargetAgtName[DA_MAXLEN_OF_AGTNAME];
	StkStringParser::ParseInto1Param(UrlPath, L"/api/timeseriesdata/$/", L'$', TargetAgtName, DA_MAXLEN_OF_AGTNAME);

	StkObject* TmpObjD = new StkObject(L"Data");

	int AgtId[DA_MAXNUM_OF_TIMESERIESDATAPERAGENT];
	long long UpdTime[DA_MAXNUM_OF_TIMESERIESDATAPERAGENT];
	int Status[DA_MAXNUM_OF_TIMESERIESDATAPERAGENT];
	int SaInterval[DA_MAXNUM_OF_TIMESERIESDATAPERAGENT];
	int Cnt = DataAccess::GetInstance()->GetTimeSeriesData(TargetAgtName, AgtId, UpdTime, Status, SaInterval);

	for (int Loop = 0; Loop < Cnt; Loop++) {
		StkObject* TmpObjC = new StkObject(L"TimeSeriesData");
		StkObject* AgtIdObj = new StkObject(L"AgentId", AgtId[Loop]);
		wchar_t UpdTimeStr[DA_MAXLEN_OF_UNIXTIME * 2 + 1] = L"";
		StkPlSwPrintf(UpdTimeStr, DA_MAXLEN_OF_UNIXTIME * 2 + 1, L"%016x", UpdTime[Loop]);
		StkObject* UpdTimeObj = new StkObject(L"UpdTime", UpdTimeStr);
		StkObject* StatusObj = new StkObject(L"Status", Status[Loop]);
		StkObject* SaIntervalObj = new StkObject(L"SaInterval", SaInterval[Loop]);
		TmpObjC->AppendChildElement(AgtIdObj);
		TmpObjC->AppendChildElement(UpdTimeObj);
		TmpObjC->AppendChildElement(StatusObj);
		TmpObjC->AppendChildElement(SaIntervalObj);
		TmpObjD->AppendChildElement(TmpObjC);
	}

	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
