#include <tchar.h>
#include "Sample_Elem1.h"

StkObject* Sample_Elem1::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* TmpObj = new StkObject(_T(""));
	StkObject* TmpObj1 = new StkObject(_T("Person"));
	StkObject* TmpObj2 = new StkObject(_T("Person"));
	StkObject* TmpObj3 = new StkObject(_T("Person"));
	TmpObj1->AppendChildElement(new StkObject(_T("name"), _T("Aaa Bbbb")));
	TmpObj1->AppendChildElement(new StkObject(_T("age"), 21));
	TmpObj1->AppendChildElement(new StkObject(_T("address"), _T("Xxx 1-1-1")));
	TmpObj2->AppendChildElement(new StkObject(_T("name"), _T("Cccc Dddd")));
	TmpObj2->AppendChildElement(new StkObject(_T("age"), 54));
	TmpObj2->AppendChildElement(new StkObject(_T("address"), _T("Yyy 2-2-2")));
	TmpObj3->AppendChildElement(new StkObject(_T("name"), _T("Eee Fffff")));
	TmpObj3->AppendChildElement(new StkObject(_T("age"), 11));
	TmpObj3->AppendChildElement(new StkObject(_T("address"), _T("Zzz 3-3-3")));
	TmpObj->AppendChildElement(TmpObj1);
	TmpObj->AppendChildElement(TmpObj2);
	TmpObj->AppendChildElement(TmpObj3);
	*ResultCode = 200;
	return TmpObj;
}
