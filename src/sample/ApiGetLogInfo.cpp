﻿#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiGetLogInfo.h"

StkObject* ApiGetLogInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	if (!CheckCredentials(Token)) {
		*ResultCode = 403;
		return NULL;
	}

	wchar_t TimeUtc[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t TimeLocal[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t Msg[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG];
	wchar_t MsgJa[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG];
	int Count = DataAccess::GetInstance()->GetLogs(TimeUtc, TimeLocal, Msg, MsgJa);

	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjD = new StkObject(L"Data");
	for (int Loop = 0; Loop < Count; Loop++) {
		StkObject* TmpObjC = new StkObject(L"Log");
		TmpObjC->AppendChildElement(new StkObject(L"Time", TimeLocal[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"MsgJa", MsgJa[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"MsgEN", Msg[Loop]));
		TmpObjD->AppendChildElement(TmpObjC);
	}
	TmpObj->AppendChildElement(TmpObjD);
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
