#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "sample_elem3.h"

StkObject* Sample_Elem3::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t Name[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	int Count = DataAccess::GetInstance()->GetAgentInfo(Name, TimeUtc, TimeLocal, UdTimeUtc, UdTimeLocal);

	StkObject* TmpObj = new StkObject(L"");
	for (int Loop = 0; Loop < Count; Loop++) {
		StkObject* TmpObjC = new StkObject(L"AgentInfo");
		TmpObjC->AppendChildElement(new StkObject(L"Name", Name[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"TimeUtc", TimeUtc[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"TimeLocal", TimeLocal[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"UdTimeUtc", UdTimeUtc[Loop]));
		TmpObjC->AppendChildElement(new StkObject(L"UdTimeLocal", UdTimeLocal[Loop]));
		TmpObj->AppendChildElement(TmpObjC);
	}
	TmpObj->AppendChildElement(new StkObject(L"Msg", L""));
	*ResultCode = 200;
	return TmpObj;
}
