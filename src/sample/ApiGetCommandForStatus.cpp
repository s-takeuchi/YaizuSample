#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "ApiGetCommandForStatus.h"

StkObject* ApiGetCommandForStatus::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* TmpObj = new StkObject(L"");
	for (int Loop = 0; Loop < 30/*60 * 15*/; Loop++) {
		StkPlSleepMs(1000);
		if (Loop == 15) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", L"Execution"));
			*ResultCode = 200;
			return TmpObj;
		}
	}
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L"Timeout"));
	*ResultCode = 200;
	return TmpObj;
}
