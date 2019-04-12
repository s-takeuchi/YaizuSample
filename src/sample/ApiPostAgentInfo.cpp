#include "ApiPostAgentInfo.h"

StkObject* ApiPostAgentInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObj1 = new StkObject(L"Person");
	StkObject* TmpObj2 = new StkObject(L"Person");
	StkObject* TmpObj3 = new StkObject(L"Person");
	TmpObj1->AppendChildElement(new StkObject(L"name", L"Aaa Bbbb"));
	TmpObj1->AppendChildElement(new StkObject(L"age", 21));
	TmpObj1->AppendChildElement(new StkObject(L"address", L"Xxx 1-1-1"));
	TmpObj2->AppendChildElement(new StkObject(L"name", L"Cccc Dddd"));
	TmpObj2->AppendChildElement(new StkObject(L"age", 54));
	TmpObj2->AppendChildElement(new StkObject(L"address", L"Yyy 2-2-2"));
	TmpObj3->AppendChildElement(new StkObject(L"name", L"Eee Fffff"));
	TmpObj3->AppendChildElement(new StkObject(L"age", 11));
	TmpObj3->AppendChildElement(new StkObject(L"address", L"Zzz 3-3-3"));
	TmpObj->AppendChildElement(TmpObj1);
	TmpObj->AppendChildElement(TmpObj2);
	TmpObj->AppendChildElement(TmpObj3);

	*ResultCode = 200;
	return TmpObj;
}
