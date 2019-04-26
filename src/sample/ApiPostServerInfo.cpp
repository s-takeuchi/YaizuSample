#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostServerInfo.h"

StkObject* ApiPostServerInfo::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	int PInterval = 0;
	int SaInterval = 0;
	if (ReqObj != NULL) {
		*ResultCode = 200;
		StkObject* ServerInfo = ReqObj->GetFirstChildElement();
		if (ServerInfo == NULL) {
			return NULL;
		}
		StkObject* PIntervalObj = ServerInfo->GetFirstChildElement();
		if (PIntervalObj == NULL) {
			return NULL;
		}
		StkObject* SaIntervalObj = PIntervalObj->GetNext();
		if (SaIntervalObj == NULL) {
			return NULL;
		}
		PInterval = PIntervalObj->GetIntValue();
		SaInterval = SaIntervalObj->GetIntValue();
		DataAccess::GetInstance()->SetServerInfo(PInterval, SaInterval);
	}

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
