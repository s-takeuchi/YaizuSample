#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiGetAgentInfo.h"

StkObject* ApiGetAgentInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	if (!CheckCredentials(Token, UserName)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	wchar_t Name[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	int Status[DA_MAXNUM_OF_AGTRECORDS];
	int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
	int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
	int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
	wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	long long AcqTime[DA_MAXNUM_OF_AGTRECORDS];
	wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	long long UpdTime[DA_MAXNUM_OF_AGTRECORDS];
	int Count = DataAccess::GetInstance()->GetAgentInfo(Name, Status, StatusCmd, OpStatus, OpCmd, TimeUtc, TimeLocal, AcqTime, UdTimeUtc, UdTimeLocal, UpdTime);

	StkObject* TmpObjD = new StkObject(L"Data");
	for (int Loop = 0; Loop < Count; Loop++) {
		StkObject* TmpObjC = new StkObject(L"AgentInfo");
		TmpObjC->AppendChildElement(new StkObject(L"Name", Name[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"Status", Status[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"StatusCmd", StatusCmd[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"OpStatus", OpStatus[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"OpCmd", OpCmd[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"TimeUtc", TimeUtc[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"TimeLocal", TimeLocal[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"UdTimeUtc", UdTimeUtc[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"UdTimeLocal", UdTimeLocal[Loop]));
		TmpObjD->AppendChildElement(TmpObjC);
	}
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
