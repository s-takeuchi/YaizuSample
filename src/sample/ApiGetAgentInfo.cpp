#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "ApiGetAgentInfo.h"

StkObject* ApiGetAgentInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	if (!CheckCredentials(Token)) {
		*ResultCode = 403;
		return NULL;
	}

	wchar_t Name[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	int Status[DA_MAXNUM_OF_AGTRECORDS];
	int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
	int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
	int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
	wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	int Count = DataAccess::GetInstance()->GetAgentInfo(Name, Status, StatusCmd, OpStatus, OpCmd, TimeUtc, TimeLocal, UdTimeUtc, UdTimeLocal);

	StkObject* TmpObj = new StkObject(L"");
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
		TmpObj->AppendChildElement(TmpObjC);
	}
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
